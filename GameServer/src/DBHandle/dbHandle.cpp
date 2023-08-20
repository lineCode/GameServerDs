#include "dbHandle.h"

#include "../../../common/datahandle/redisPool.h"
#include "spdlog/spdlog.h"

namespace MiniProject
{

    uint32_t DbHandle::GetUserUid(const std::string &_username)
    {

        spdlog::info("GetUserUid for {}", _username);

        std::string _key = USER_INFO_PREFIX + _username;

        RedisConnectionRaII redisRall(RedisConnectionPool::GetInstance());

        std::string _val;
        int ret = redisRall.get(_key, _val);

        if (ret == 1)
        {
            return atol(_val.c_str());
        }

        return 0;
    }

    bool DbHandle::CacheUserInfo(const std::string &_username, uint32_t _uid, int _exseconds)
    {
        spdlog::info("CacheUserInfo {}", _uid);

        std::string _key = USER_INFO_PREFIX + _username;

        RedisConnectionRaII redisRall(RedisConnectionPool::GetInstance());

        std::string _val = std::to_string(_uid);
        int ret = redisRall.set(_key, _val, _exseconds);

        return ret == 1;
    }

    

}