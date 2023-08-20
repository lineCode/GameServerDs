#ifndef MiniProjectSqlPool_H
#define MiniProjectSqlPool_H

#include "../include/lock.hpp"
#include <error.h>
#include <iostream>
#include <list>
#include <mutex>
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <vector>

namespace MiniProject {
class connection_pool {
public:
  MYSQL *GetConnection();              // 获取数据库连接
  bool ReleaseConnection(MYSQL *conn); // 释放连接
  int GetFreeConn();                   // 获取连接
  void DestroyPool();                  // 销毁所有连接

  // 单例模式
  static connection_pool *GetInstance();

  void init(std::string url, std::string User, std::string PassWord,
            std::string DataBaseName, int Port, int MaxConn = 8);

private:
  connection_pool();
  ~connection_pool();

  int m_MaxConn;  // 最大连接数
  int m_CurConn;  // 当前已使用的连接数
  int m_FreeConn; // 当前空闲的连接数
  locker lock;
  std::list<MYSQL *> connList; // 连接池
  sem reserve;

public:
  std::string m_url;          // 主机地址
  std::string m_Port;         // 数据库端口号
  std::string m_User;         // 登陆数据库用户名
  std::string m_PassWord;     // 登陆数据库密码
  std::string m_DatabaseName; // 使用数据库名
};

class connectionRAII {

  class connectionRAIIProxy {
  public:
    // 构造函数
    connectionRAIIProxy(connectionRAII *base) : base_(base) {}
    // 通过下标找到指定列
    // idx:列下标
    // 返回值：指定列的值
    std::string operator[](unsigned int idx) const;

    // 通过关键字找到指定列
    // key:列名称关键字
    // 返回值：指定列的值
    std::string operator[](const std::string &key) const;

  private:
    // 用于访问CMysql类的对象
    connectionRAII *base_;
  };

public:
  connectionRAII(connection_pool *connPool);
  ~connectionRAII();

  int MysqlInsertWrap(const std::string &query);

  int MysqlQueryWrap(const std::string &query);

  int MysqlDeleteWrap(const std::string &query);

  int MysqlUpdateWrap(const std::string &query);

  connectionRAIIProxy operator[](unsigned int idx);

private:
  unsigned int MysqlFieldCountWrap();

private:
  unsigned int m_int_field_;
  unsigned int m_int_rows_;
  MYSQL *conRAII;
  MYSQL_RES *res;
  MYSQL_ROW sqlrow_;
  std::vector<std::string> colname_;

private:
  connection_pool *poolRAII;
};
} // namespace MiniProject

#endif