#ifndef MINIPROJECT_DBHANLE_HPP
#define MINIPROJECT_DBHANLE_HPP

#include <string>

namespace MiniProject
{
    class DbHandle
    {
    public:
        static uint32_t GetVerification(const std::string &username, std::string &passwd);

        static bool RegisterUser(const std::string &username, const std::string &passwd);
    };

}

#endif