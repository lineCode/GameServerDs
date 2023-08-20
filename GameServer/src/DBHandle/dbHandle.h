#ifndef MINIPROJECT_GAME_DBHANDLE_HPP
#define MINIPROJECT_GAME_DBHANDLE_HPP

#include <string>
#include <google/protobuf/message.h>
#include <memory>

namespace MiniProject
{
    const std::string USER_INFO_PREFIX = "USERINFO::";
    const std::string USER_ONLINE_PREFIX = "USERONLINE::";

    class DbHandle
    {
    public:
        static uint32_t GetUserUid(const std::string &_username);

        static bool CacheUserInfo(const std::string &_username, uint32_t _uid, int _exseconds = 60 * 10);

    };
}
#endif