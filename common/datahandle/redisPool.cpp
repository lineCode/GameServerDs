#include "redisPool.h"

#include "spdlog/spdlog.h"
#include <cassert>
#include <cstring>
#include <sstream>

namespace MiniProject
{

    RedisConnectionPool *RedisConnectionPool::GetInstance()
    {
        static RedisConnectionPool connPool;
        return &connPool;
    }

    RedisConnectionPool::RedisConnectionPool()
    {
        m_CurConn = 0;
        m_FreeConn = 0;
    }

    /*********************************************************
    Function:	GetConnection
    Description:获取redis的句柄
    Input:addr_:
    Output:
    Return:redisContext:redis的句柄
    **********************************************************/
    redisContext *RedisConnectionPool::GetConnection()
    {
        redisContext *con = nullptr;

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

    /*********************************************************
    Function:	GetFreeConn
    Description:获取当前空闲的连接数
    Input:addr_:
    Output:
    Return:int:空闲连接数
    **********************************************************/
    int RedisConnectionPool::GetFreeConn() { return this->m_FreeConn; }

    /*********************************************************
    Function:	~connection_pool
    Description:析构函数，释放连接
    Input:
    Output:
    Return:
    **********************************************************/
    RedisConnectionPool::~RedisConnectionPool() { DestroyPool(); }

    /*********************************************************
    Function:	~DestroyPool
    Description:释放连接
    Input:
    Output:
    Return:
    **********************************************************/
    void RedisConnectionPool::DestroyPool()
    {
        lock.lock();
        if (connList.size() > 0)
        {
            std::list<redisContext *>::iterator it;
            for (it = connList.begin(); it != connList.end(); ++it)
            {
                redisContext *con = *it;
                redisFree(con);
            }
            m_CurConn = 0;
            m_FreeConn = 0;
            connList.clear();
        }
        lock.unlock();
    }

    /*********************************************************
    Function:	ReleaseConnection
    Description:释放redis的句柄
    Input:addr_:
    Output:
    Return:bool:操作结果
    **********************************************************/
    bool RedisConnectionPool::ReleaseConnection(redisContext *conn)
    {
        if (nullptr == conn)
            return false;

        lock.lock();

        connList.push_back(conn);
        ++m_FreeConn;
        --m_CurConn;

        lock.unlock();

        reserve.post();
        return true;
    }

    /*********************************************************
    Function:	init
    Description:RedisPool的初始化
    Input:addr_:redis服务的地址，port:端口号，pwd：密码，MaxConn：最大连接数
    Output:
    Return:OperationStatus:用户注册结果
    **********************************************************/
    void RedisConnectionPool::init(const std::string &addr_, int port,
                                   const std::string &pwd, int MaxConn)
    {
        m_url = addr_;
        m_Port = port;
        m_PassWord = pwd;

        for (int i = 0; i < MaxConn; ++i)
        {
            redisContext *_context = redisConnect(addr_.c_str(), port);
            if (_context == NULL || _context->err)
            {
                if (_context)
                {
                    spdlog::error("Redis connect Error:{}\n", _context->errstr);
                    redisFree(_context);
                }
                else
                {
                    spdlog::error("can't allocate redis context\n");
                }
                exit(1);
            }
            bool _auth_ret = auth(_context, pwd);
            assert(_auth_ret);
            connList.push_back(_context);
            ++m_FreeConn;
        }

        reserve = sem(m_FreeConn);

        m_MaxConn = m_FreeConn;
    }

    /*********************************************************
    Function:	auth
    Description:登录redis-server
    Input:_context:redis上下文，pwd_:redis-server的密码
    Output:
    Return:bool：初始化是否成功
    **********************************************************/
    bool RedisConnectionPool::auth(redisContext *_context,
                                   const std::string &pwd_)
    {
        if (_context == nullptr)
        {
            return false;
        }
        redisReply *reply =
            (redisReply *)redisCommand(_context, "AUTH %s", pwd_.c_str());
        if (nullptr == reply)
        {
            return false;
        }
        if (strcmp(reply->str, "OK") != 0)
        {
            freeReplyObject(reply);
            return false;
        }
        freeReplyObject(reply);
        return true;
    }

    /*********************************************************
    Function:	RedisConnectionRaII
    Description:构造RedisConnectionRaII，并获取一个redis连接
    Input:_context:redis上下文，pwd_:redis-server的密码
    Output:
    Return:bool：初始化是否成功
    **********************************************************/
    RedisConnectionRaII::RedisConnectionRaII(RedisConnectionPool *base)
        : poolRaII(base), _context(base->GetConnection()) {}

    /*********************************************************
    Function:	~RedisConnectionRaII
    Description:析构函数，释放redis上下文
    Input:_context:redis上下文，pwd_:redis-server的密码
    Output:
    Return:bool：初始化是否成功
    **********************************************************/
    RedisConnectionRaII::~RedisConnectionRaII()
    {
        // for (auto &_key : m_lock_keys)
        // {
        //   unlock(_key);
        // }
        poolRaII->ReleaseConnection(_context);
        _context = nullptr;
        poolRaII = nullptr;
    }

    int RedisConnectionRaII::LOCK_TIME = 10;

    /*********************************************************
    Function:	lock
    Description:获取redis的分布锁
    Input:_key:待获取分布锁的id
    Output:
    Return:int:是否获取成功
    **********************************************************/
    int RedisConnectionRaII::lock(const std::string &_key)
    {
        // if (m_set_keys.count(_key))
        // {
        //   return -1;
        // }
        // redisReply *reply = static_cast<redisReply *>(redisCommand(_context, "SET %s %d NX PX %d", _key.c_str(), 1, LOCK_TIME));
        // if (!reply)
        // {
        //   return -1;
        // }
        // int ret = strcmp(reply->str, "OK") == 0;
        // freeReplyObject(reply);
        // m_set_keys.insert(_key);
        // return ret;
        return 0;
    }

    /*********************************************************
    Function:	unlock
    Description:释放redis的分布锁
    Input:_key:待释放分布锁的id
    Output:
    Return:int：是否释放成功
    **********************************************************/
    int RedisConnectionRaII::unlock(const std::string &_key)
    {
        // if (!m_set_keys.count(_key))
        // {
        //   return -1;
        // }
        // redisReply *reply = static_cast<redisReply *>(redisCommand(_context, "GET %s", _key.c_str()));
        // if (!reply)
        // {
        //   return -1;
        // }
        // if (strcmp(reply->str, "1") == 0)
        // {
        //   freeReplyObject(reply);
        //   reply = static_cast<redisReply *>(redisCommand(_context, "DEL %s", _key.c_str()));
        //   if (!reply)
        //   {
        //     return -1;
        //   }
        //   freeReplyObject(reply);
        //   m_set_keys.erase(_key);
        //   return 1;
        // }
        // freeReplyObject(reply);
        return 0;
    }

    /*********************************************************
    Function:	set
    Description:设置键值
    Input:key：键，val:值，expire：超时时间
    Output:
    Return:int:是否设置成功
    **********************************************************/
    int RedisConnectionRaII::set(const std::string &key, const std::string &val,
                                 int expire)
    {
        redisReply *reply = nullptr;
        if (expire > 0)
        {
            reply = static_cast<redisReply *>(redisCommand(
                _context, "SET %s %s EX %d", key.c_str(), val.c_str(), expire));
        }
        else
        {
            reply = static_cast<redisReply *>(
                redisCommand(_context, "SET %s %s", key.c_str(), val.c_str()));
        }
        if (reply == nullptr)
        {
            return -1; // SET 操作失败
        }
        int result =
            reply->type == REDIS_REPLY_STATUS && strcmp(reply->str, "OK") == 0 ? 1
                                                                               : 0;
        freeReplyObject(reply);
        return result;
    }
    /*********************************************************
    Function:	get
    Description:查询键的值
    Input:key：键，val:值
    Output:
    Return:int：是否查询成功
    **********************************************************/
    int RedisConnectionRaII::get(const std::string &key, std::string &val)
    {
        redisReply *reply =
            static_cast<redisReply *>(redisCommand(_context, "GET %s", key.c_str()));
        if (reply == nullptr)
        {
            return -1;
        }
        int ret = 0;
        if (reply->type == REDIS_REPLY_STRING)
        {
            val = std::string((const char *)(reply->str), reply->len);
            ret = 1;
        }
        freeReplyObject(reply);
        return ret;
    }

    int RedisConnectionRaII::expire(const std::string &key, int expire)
    {
        redisReply *reply =
            static_cast<redisReply *>(redisCommand(_context, "EXPIRE %s %d", key.c_str(),expire));
        if (reply == nullptr)
        {
            return -1;
        }
        int ret = 0;
        if (reply->type == REDIS_REPLY_INTEGER)
        {
            ret = (int)reply->integer;
        }
        return ret;
    }

    /*********************************************************
    Function:	del
    Description:删除键
    Input:key
    Output:
    Return:int:是否删除成功
    **********************************************************/
    int RedisConnectionRaII::del(const std::string &key)
    {
        redisReply *reply =
            static_cast<redisReply *>(redisCommand(_context, "DEL %s", key.c_str()));
        if (reply == nullptr)
        {
            return -1;
        }
        int ret = (int)reply->integer;
        freeReplyObject(reply);
        return ret;
    }
    /*********************************************************
    Function:	lPop
    Description:删除list的最左边的值
    Input:key：待操作list的key
    Output:
    Return:int:是否删除成功
    **********************************************************/
    int RedisConnectionRaII::lPop(const std::string &key)
    {
        redisReply *reply =
            static_cast<redisReply *>(redisCommand(_context, "LPOP %s", key.c_str()));

        if (reply == nullptr)
        {
            return -1;
        }
        int ret = 0;
        if (reply->type == REDIS_REPLY_INTEGER)
        {
            ret = (int)reply->integer;
        }
        return ret;
    }

    /*********************************************************
    Function:	rPush
    Description:从右边插入list
    Input:key：待操作list的key，char：数据，len：数据长度
    Output:
    Return:int:是否插入成功
    **********************************************************/
    int RedisConnectionRaII::rPush(const std::string &key, char *val, int len)
    {
        redisReply *reply = static_cast<redisReply *>(
            redisCommand(_context, "RPUSH %s %b", key.c_str(), val, len));
        if (reply == nullptr)
        {
            return -1;
        }
        int ret = 0;
        if (reply->type == REDIS_REPLY_INTEGER)
        {
            ret = (int)reply->integer;
        }
        return ret;
    }

    /*********************************************************
    Function:	lrange
    Description:查询某个list的成员
    Input:key：待操作list的key, l,r:查询索引的范围，res：用以存储查询的结果
    Output:
    Return:int:查询到的数量
    **********************************************************/
    int RedisConnectionRaII::lrange(const std::string &key, int l, int r,
                                    std::list<std::string> &res)
    {
        redisReply *reply = static_cast<redisReply *>(
            redisCommand(_context, "LRANGE %s %d %d", key.c_str(), l, r));
        if (reply == nullptr)
        {
            return -1;
        }
        int ret = 0;
        if (reply->type == REDIS_REPLY_ARRAY)
        {
            ret = reply->elements;
            res.clear();
            for (int i = 0; i < ret; ++i)
            {
                res.push_back(
                    std::string(reply->element[i]->str, reply->element[i]->len));
            }
        }
        return ret;
    }

    /*********************************************************
    Function:	sAdd
    Description:添加成员到set中
    Input:keySet：SET的key，key：待添加成员的key
    Output:
    Return:int:是否插入成功
    **********************************************************/
    int RedisConnectionRaII::sAdd(const std::string &keySet,
                                  const std::string &key)
    {
        redisReply *reply = static_cast<redisReply *>(
            redisCommand(_context, "SADD %s %s", keySet.c_str(), key.c_str()));
        if (reply == nullptr)
        {
            return -1;
        }
        int ret = 0;
        if (reply->type == REDIS_REPLY_INTEGER)
        {
            ret = (int)reply->integer;
        }
        return ret;
    }

    /*********************************************************
    Function:	sDel
    Description:删除set中的某个成员
    Input:keySet：SET的key，key：待删除成员的key
    Output:
    Return:int:是否删除成功
    **********************************************************/
    int RedisConnectionRaII::sRem(const std::string &keySet,
                                  const std::string &key)
    {
        redisReply *reply = static_cast<redisReply *>(
            redisCommand(_context, "SREM %s %s", keySet.c_str(), key.c_str()));
        if (reply == nullptr)
        {
            return -1;
        }
        int ret = 0;
        if (reply->type == REDIS_REPLY_INTEGER)
        {
            ret = (int)reply->integer;
        }
        return ret;
    }

    /*********************************************************
    Function:	sMembers
    Description:查询某set中的成员
    Input:key：待操作list的key res：存储查询的成员
    Output:
    Return:int:是否插入成功
    **********************************************************/
    int RedisConnectionRaII::sMembers(const std::string &key,
                                      std::list<std::string> &res)
    {
        redisReply *reply = static_cast<redisReply *>(
            redisCommand(_context, "SMEMBERS %s ", key.c_str()));
        if (reply == nullptr)
        {
            return -1;
        }
        int ret = 0;
        if (reply->type == REDIS_REPLY_ARRAY)
        {
            ret = reply->elements;
            res.clear();
            for (int i = 0; i < ret; ++i)
            {
                res.push_back(
                    std::string(reply->element[i]->str, reply->element[i]->len));
            }
        }
        return ret;
    }

    /*********************************************************
    Function:	rPush
    Description:从右边插入list
    Input:key：待操作list的key，char：数据，len：数据长度
    Output:
    Return:int:是否插入成功
    **********************************************************/
    int RedisConnectionRaII::sIsMembers(const std::string &keySet,
                                        const std::string &ketMembers)
    {
        redisReply *reply = static_cast<redisReply *>(redisCommand(
            _context, "SISMEMBER %s %s", keySet.c_str(), ketMembers.c_str()));
        if (reply == nullptr)
        {
            return -1;
        }
        int ret = 0;
        if (reply->type == REDIS_REPLY_INTEGER)
        {
            ret = (int)reply->integer;
        }
        return ret;
    }

    /*********************************************************
    Function:	hlen
    Description:获取哈希表中的键的数量
    Input:keyMap：待查询的哈希表键
    Output:
    Return:int:键的数量
    **********************************************************/
    int RedisConnectionRaII::hlen(const std::string &keyMap)
    {
        redisReply *reply = static_cast<redisReply *>(redisCommand(
            _context, "HLEN %s", keyMap.c_str()));
        if (reply == nullptr)
        {
            return -1;
        }
        int ret = 0;
        if (reply->type == REDIS_REPLY_INTEGER)
        {
            ret = (int)reply->integer;
        }
        return ret;
    }

    /*********************************************************
    Function:	hSet
    Description:设置哈希表中的键值
    Input:keyMap：待操作的哈希表key，key，val
    Output:
    Return:int:是否插入成功
    **********************************************************/
    int RedisConnectionRaII::hSet(const std::string &keyMap, const std::string &key,
                                  const std::string &val)
    {
        redisReply *reply = static_cast<redisReply *>(
            redisCommand(_context, "HSET %s %s %s", keyMap.c_str(),
                         key.c_str(), val.c_str()));
        if (reply == nullptr)
        {
            return -1;
        }
        int ret = 0;
        if (reply->type == REDIS_REPLY_INTEGER)
        {
            ret = (int)reply->integer;
        }
        return ret;
    }

    /*********************************************************
    Function:	hDel
    Description:删除哈希表中的键
    Input:keyMap：待操作的哈希表key，key
    Output:
    Return:int:是否插入成功
    **********************************************************/
    int RedisConnectionRaII::hDel(const std::string &keyMap,
                                  const std::string &key)
    {
        redisReply *reply = static_cast<redisReply *>(
            redisCommand(_context, "HDEL %s %s ", keyMap.c_str(), key.c_str()));
        if (reply == nullptr)
        {
            return -1;
        }
        int ret = 0;
        if (reply->type == REDIS_REPLY_INTEGER)
        {
            ret = (int)reply->integer;
        }
        return ret;
    }

    /*********************************************************
    Function:	hDel
    Description:删除哈希表中的键
    Input:keyMap：待操作的哈希表key，key
    Output:
    Return:int:是否插入成功
    **********************************************************/
    int RedisConnectionRaII::hDel(const std::string &keyMap,
                                  const std::list<std::string> &keys)
    {
        std::stringstream stream;

        stream << "HDEL " << keyMap;

        for (auto &key : keys)
        {
            stream << " " << key;
        }

        std::string _str = stream.str();

        redisReply *reply = static_cast<redisReply *>(
            redisCommand(_context, "%b", _str.c_str(), _str.size()));
        if (reply == nullptr)
        {
            return -1;
        }
        int ret = 0;
        if (reply->type == REDIS_REPLY_INTEGER)
        {
            ret = (int)reply->integer;
        }
        return ret;
    }

    /*********************************************************
    Function:	hGet
    Description:获取哈希表键的值
    Input:keyMap：待操作的哈希表key，key
    Output:
    Return:int:查询结果
    **********************************************************/
    int RedisConnectionRaII::hGet(const std::string &keyMap, const std::string &key,
                                  std::string &val)
    {
        redisReply *reply = static_cast<redisReply *>(
            redisCommand(_context, "HGET %s %s ", keyMap.c_str(), key.c_str()));
        if (reply == nullptr)
        {
            return -1;
        }
        int ret = 0;
        if (reply->type == REDIS_REPLY_STRING)
        {
            val = std::string((const char *)(reply->str), reply->len);
            ret = 1;
        }
        return ret;
    }

    /*********************************************************
    Function:	hGet
    Description:获取哈希表键的值
    Input:keyMap：待操作的哈希表key，key
    Output:
    Return:int:查询结果
    **********************************************************/
    int RedisConnectionRaII::hGetAll(const std::string &keyMap, std::list<std::pair<std::string, std::string>> &val)
    {
        redisReply *reply = static_cast<redisReply *>(
            redisCommand(_context, "HGETALL %s", keyMap.c_str()));
        if (reply == nullptr || reply->type == REDIS_REPLY_ERROR)
        {
            return -1;
        }
        int ret = 0;
        if (reply->type == REDIS_REPLY_ARRAY)
        {
            ret = reply->elements;
            val.clear();
            for (int i = 0; i < ret; i += 2)
            {
                val.push_back({std::string(reply->element[i]->str, reply->element[i]->len), std::string(reply->element[i + 1]->str, reply->element[i + 1]->len)});
            }
        }
        return ret;
    }

} // namespace MiniProject