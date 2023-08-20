#ifndef MINIPROJECT_GAME_USERMANAGER_HPP
#define MINIPROJECT_GAME_USERMANAGER_HPP

#include "../typedef.h"
#include "User.h"
#include <boost/noncopyable.hpp>
#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

namespace MiniProject
{
    class UserManager : public boost::noncopyable
    {
    public:
        static UserManager *Instance()
        {
            static UserManager _rManager;
            return &_rManager;
        }

        bool addUserById(User::UserPtr _ptr);
        void RemoveUserById(const UserId &_id);
        User::UserPtr findUserById(const UserId &_uid);

    private:
        std::unordered_map<UserId, User::UserPtr> userMaps_;
        std::mutex userMutex_;
    };

}

#endif