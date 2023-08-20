#ifndef MINIPROJECT_USERMANAGER_HPP
#define MINIPROJECT_USERMANAGER_HPP

#include "User.h"
#include <memory>
#include <boost/noncopyable.hpp>
#include <unordered_map>
#include <list>
#include <unordered_set>
#include <mutex>

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

        User::UserPtr findUserById(UserId _uid);
        User::UserPtr findUserByConv(NetId _conv);

        bool AddUserById(const UserId &_uid, User::UserPtr _ptr);
        void RemoveUserById(const UserId &_uid);
        void RemoveOnlyId(const UserId &_uid);

        bool AddUserByConv(const NetId &_uid, User::UserPtr _ptr);
        void RemoveUserByConv(const NetId &_conv);
        void RemoveOnlyConv(const NetId &_conv);

        std::list<User::UserPtr> GetAllUsers()
        {
            std::lock_guard<std::mutex> lock_(m_mutex_users_);
            return m_list_user_ptrs_;
        }

    private:
        std::mutex m_mutex_users_;

        std::list<User::UserPtr> m_list_user_ptrs_;
        std::unordered_map<UserId, std::list<User::UserPtr>::iterator> m_map_id_users_;
        std::unordered_map<NetId, std::list<User::UserPtr>::iterator> m_map_conv_users;
    };

}

#endif