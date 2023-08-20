#ifndef MiniProjectRedisPool_H
#define MiniProjectRedisPool_H

#include "../include/lock.hpp"
#include <error.h>
#include "hiredis.h"
#include <iostream>
#include <list>
#include <mutex>
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <set>

namespace MiniProject
{
  class RedisConnectionPool
  {
  public:
    redisContext *GetConnection();              // 获取数据库连接
    bool ReleaseConnection(redisContext *conn); // 释放连接
    int GetFreeConn();                          // 获取连接
    void DestroyPool();                         // 销毁所有连接

    // 单例模式
    static RedisConnectionPool *GetInstance();

    void init(const std::string &addr_, int port, const std::string &pwd,
              int MaxConn = 8);

  private:
    RedisConnectionPool();
    ~RedisConnectionPool();

    bool auth(redisContext *_context, const std::string &pwd_);

    int m_MaxConn;  // 最大连接数
    int m_CurConn;  // 当前已使用的连接数
    int m_FreeConn; // 当前空闲的连接数
    locker lock;
    std::list<redisContext *> connList; // 连接池
    sem reserve;

  public:
    std::string m_url;      // 主机地址
    std::string m_Port;     // 数据库端口号
    std::string m_PassWord; // 登陆数据库密码
  };

  class RedisConnectionRaII
  {
  public:
    RedisConnectionRaII(RedisConnectionPool *base);
    ~RedisConnectionRaII();

    int lock(const std::string &_key);
    int unlock(const std::string &_key);
    int set(const std::string &key, const std::string &val, int expire = -1);
    int get(const std::string &key, std::string &val);
    int expire(const std::string &key, int expire);
    int del(const std::string &key);
    int lPop(const std::string &key);
    int rPush(const std::string &key, char *val, int len);
    int lrange(const std::string &key, int l, int r, std::list<std::string> &res);
    int sAdd(const std::string &keySet, const std::string &key);
    int sRem(const std::string &keySet, const std::string &key);
    int sMembers(const std::string &key, std::list<std::string> &res);
    int sIsMembers(const std::string &keySet, const std::string &ketMembers);
    int hlen(const std::string &keyMap);
    int hSet(const std::string &keyMap, const std::string &key,
             const std::string &val);
    int hDel(const std::string &keyMap, const std::string &key);
    int hGet(const std::string &keyMap, const std::string &key, std::string &val);
    int hGetAll(const std::string &keyMap, std::list<std::pair<std::string, std::string>> &val);
    int hDel(const std::string &keyMap, const std::list<std::string> &keys);

  private:
    static int LOCK_TIME;
    RedisConnectionPool *poolRaII;
    redisContext *_context;
    // std::set<const std::string> m_set_keys;
  };

} // namespace MiniProject

#endif