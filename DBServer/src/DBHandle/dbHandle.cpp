#include "dbHandle.h"
#include "../../../common/datahandle/mysqlPool.h"
#include "../../../common/datahandle/redisPool.h"
#include <sstream>
#include "spdlog/spdlog.h"

namespace MiniProject
{
    uint32_t DbHandle::GetVerification(const std::string &username, std::string &passwd)
    {
        // const std::string prefix = "LOGIN_SERVER::";

        // spdlog::info("GetVerification for {}", username);

        // RedisConnectionRaII redisRall(RedisConnectionPool::GetInstance());

        // std::string val;
        // int ret = redisRall.get(prefix + username, val);
        // if (ret == 0)
        // {
        //     return atoll(val.c_str());
        // }

        std::stringstream stream;

        stream << "select uid,username,passwd from t_user_back where username = '"
               << username << "' lock in share mode";

        std::string query = stream.str();

        connectionRAII cRaII(connection_pool::GetInstance());

        int ret = cRaII.MysqlQueryWrap(query);

        if (ret <= 0)
        {
            return 0;
        }

        uint32_t _uid = atol(cRaII[0]["uid"].c_str());
        passwd = cRaII[0]["passwd"].c_str();

        return _uid;
    }

    bool DbHandle::RegisterUser(const std::string &username, const std::string &passwd)
    {
        std::stringstream stream;

        stream << "INSERT INTO t_user_back(username,passwd) values('" << username << "','" << passwd << "')";

        connectionRAII cRaII(connection_pool::GetInstance());

        std::string query = stream.str();

        return cRaII.MysqlInsertWrap(query) > 0;
    }

}