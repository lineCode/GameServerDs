#include "mysqlPool.h"

#include "spdlog/spdlog.h"

namespace MiniProject {

connection_pool::connection_pool() {
  m_CurConn = 0;
  m_FreeConn = 0;
}

connection_pool *connection_pool::GetInstance() {
  static connection_pool connPool;
  return &connPool;
}

// 构造初始化
void connection_pool::init(std::string url, std::string User,
                           std::string PassWord, std::string DBName, int Port,
                           int MaxConn) {
  m_url = url;
  m_Port = Port;
  m_User = User;
  m_PassWord = PassWord;
  m_DatabaseName = DBName;

  for (int i = 0; i < MaxConn; i++) {
    MYSQL *con = NULL;
    con = mysql_init(con);

    if (con == NULL) {
      spdlog::error("MySQL Error");
      exit(1);
    }
    con = mysql_real_connect(con, url.c_str(), User.c_str(), PassWord.c_str(),
                             DBName.c_str(), Port, NULL, 0);

    if (con == NULL) {
      spdlog::error("MySQL Error");
      exit(1);
    }
    connList.push_back(con);
    ++m_FreeConn;
  }

  reserve = sem(m_FreeConn);

  m_MaxConn = m_FreeConn;
}

// 当有请求时，从数据库连接池中返回一个可用连接，更新使用和空闲连接数
MYSQL *connection_pool::GetConnection() {
  MYSQL *con = NULL;

  if (0 == connList.size())
    return NULL;

  reserve.wait();

  lock.lock();

  con = connList.front();
  connList.pop_front();

  --m_FreeConn;
  ++m_CurConn;

  lock.unlock();
  return con;
}

// 释放当前使用的连接
bool connection_pool::ReleaseConnection(MYSQL *con) {
  if (NULL == con)
    return false;

  lock.lock();

  connList.push_back(con);
  ++m_FreeConn;
  --m_CurConn;

  lock.unlock();

  reserve.post();
  return true;
}

// 销毁数据库连接池
void connection_pool::DestroyPool() {

  lock.lock();
  if (connList.size() > 0) {
    std::list<MYSQL *>::iterator it;
    for (it = connList.begin(); it != connList.end(); ++it) {
      MYSQL *con = *it;
      mysql_close(con);
    }
    m_CurConn = 0;
    m_FreeConn = 0;
    connList.clear();
  }

  lock.unlock();
}

// 当前空闲的连接数
int connection_pool::GetFreeConn() { return this->m_FreeConn; }

connection_pool::~connection_pool() { DestroyPool(); }

connectionRAII::connectionRAII(connection_pool *connPool)
    : m_int_field_(0), conRAII(connPool->GetConnection()), res(nullptr),
      poolRAII(connPool) {}

connectionRAII::~connectionRAII() {
  poolRAII->ReleaseConnection(conRAII);
  conRAII = nullptr;
  res = nullptr;
}

unsigned int connectionRAII::MysqlFieldCountWrap() {
  if (conRAII != NULL && res != NULL) {
    return mysql_field_count(conRAII);
  }
  return 0;
}

std::string
connectionRAII::connectionRAIIProxy::operator[](unsigned int idx) const {

  // 数据对象为空
  if (base_->res == NULL) {
    return std::string("");
  }

  if (idx >= base_->m_int_field_ || idx < 0) {
    return std::string("");
  }

  base_->sqlrow_ = mysql_fetch_row(base_->res);
  unsigned long *length = mysql_fetch_lengths(base_->res);
  std::string strData(length[idx], '\0');
  memcpy((void *)strData.c_str(), base_->sqlrow_[idx], length[idx]);
  // 返回指定列
  return strData;
}

connectionRAII::connectionRAIIProxy
connectionRAII::operator[](unsigned int idx) {
  // 数据对象为空
  if (res == NULL) {
    return connectionRAIIProxy(this);
  }

  // 判断下标是否超出行范围
  if (idx >= m_int_rows_ || idx < 0) {
    idx = 0;
  }

  mysql_data_seek(res, idx);

  return connectionRAIIProxy(this);
}

std::string
connectionRAII::connectionRAIIProxy::operator[](const std::string &key) const {

  if (base_->res == NULL) {
    return std::string("");
  }

  // 查找给定关键字列
  unsigned int idx = 0;
  std::vector<std::string>::const_iterator iter;
  for (iter = base_->colname_.begin(); iter != base_->colname_.end(); ++iter) {
    if (key == *iter) {
      break;
    }
    ++idx;
  }

  // 未找到给定关键字的列
  if (iter == base_->colname_.end()) {
    return std::string("");
  }

  return (*this)[idx];
}

int connectionRAII::MysqlInsertWrap(const std::string &query) {
  if (conRAII != nullptr && !query.empty()) {
    if (mysql_query(conRAII, query.c_str()) == 0) {
      my_ulonglong rows = mysql_affected_rows(conRAII);
      return rows;
    } else {
      char sz_errno[16];
      sprintf(sz_errno, "%d", mysql_errno(conRAII));
      spdlog::error("MysqlInsertWrap error {}", sz_errno);
      return -1;
    }
  }
  return -1;
}

int connectionRAII::MysqlQueryWrap(const std::string &query) {
  m_int_field_ = 0;

  if (conRAII == nullptr || query.empty())
    return -1;

  if (mysql_query(conRAII, query.c_str()) != 0) {
    return -1;
  }
;
  if ((res = mysql_store_result(conRAII)) == NULL) {
    return -1;
  }

  MYSQL_FIELD *fieldPtr = NULL;
  while ((fieldPtr = mysql_fetch_field(res)) != NULL) {
    colname_.push_back(fieldPtr->name);
    ++m_int_field_;
  }

  m_int_rows_ = mysql_num_rows(res);

  return m_int_rows_;
}

int connectionRAII::MysqlDeleteWrap(const std::string &query) {
  if (conRAII != nullptr && !query.empty()) {
    if (mysql_query(conRAII, query.c_str()) == 0)
      return 0;
    else {
      char sz_errno[16];
      sprintf(sz_errno, "%d", mysql_errno(conRAII));
      spdlog::error("MysqlDeleteWrap error {}", sz_errno);
      return -1;
    }
  }
  return -1;
}

int connectionRAII::MysqlUpdateWrap(const std::string &query) {
  return conRAII == nullptr || query.empty() ||
                 mysql_query(conRAII, query.c_str())
             ? -1
             : 0;
}

} // namespace MiniProject