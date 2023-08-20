#include "UserManager.h"

namespace MiniProject
{

    User::UserPtr UserManager::findUserById(UserId _uid)
    {
        std::lock_guard<std::mutex> lock_(m_mutex_users_);

        auto it = m_map_id_users_.find(_uid);

        if (it != m_map_id_users_.end())
        {
            return *it->second;
        }

        return User::UserPtr();
    }

    User::UserPtr UserManager::findUserByConv(NetId _conv)
    {
        std::lock_guard<std::mutex> lock_(m_mutex_users_);

        auto it = m_map_conv_users.find(_conv);

        if (it != m_map_conv_users.end())
        {
            return *it->second;
        }

        return User::UserPtr();
    }

    bool UserManager::AddUserById(const UserId &_uid, User::UserPtr _ptr)
    {
        std::lock_guard<std::mutex> lock_(m_mutex_users_);

        auto it = m_map_id_users_.find(_uid);

        if (it != m_map_id_users_.end())
        {
            return false;
        }

        NetId _conv = _ptr->ConvId();
        if (_conv != 0)
        {
            auto itconv = m_map_conv_users.find(_conv);

            if (itconv != m_map_conv_users.end())
            {
                m_map_id_users_[_uid] = itconv->second;
            }
            else
            {
                m_list_user_ptrs_.push_back(_ptr);
                m_map_id_users_[_uid] = std::prev(m_list_user_ptrs_.end());
                m_map_conv_users[_conv] = std::prev(m_list_user_ptrs_.end());
            }
            return true;
        }
        m_list_user_ptrs_.push_back(_ptr);
        m_map_id_users_[_uid] = std::prev(m_list_user_ptrs_.end());
        return true;
    }

    void UserManager::RemoveUserById(const UserId &_uid)
    {
        std::lock_guard<std::mutex> lock_(m_mutex_users_);

        auto it = m_map_id_users_.find(_uid);

        if (it == m_map_id_users_.end())
        {
            return;
        }

        uint _conv = (*it->second)->ConvId();

        if (_conv != 0)
        {
            m_map_conv_users.erase(_conv);
        }

        m_list_user_ptrs_.erase(it->second);
        m_map_id_users_.erase(it);
    }

    void UserManager::RemoveOnlyId(const UserId &_uid)
    {
        std::lock_guard<std::mutex> lock_(m_mutex_users_);

        auto it = m_map_id_users_.find(_uid);

        if (it == m_map_id_users_.end())
        {
            return;
        }
        (*(it->second))->set_user_id(0);

        // if ((*(it->second))->ConvId() == 0)
        // {
        //     m_list_user_ptrs_.remove(it->second);
        // }

        m_map_id_users_.erase(it);
    }

    bool UserManager::AddUserByConv(const NetId &_conv, User::UserPtr _ptr)
    {
        std::lock_guard<std::mutex> lock_(m_mutex_users_);

        auto it = m_map_conv_users.find(_conv);

        if (it != m_map_conv_users.end())
        {
            return false;
        }

        UserId _uid = _ptr->userId();

        if (_uid != 0)
        {
            auto itUid = m_map_id_users_.find(_uid);

            if (itUid != m_map_id_users_.end())
            {
                m_map_conv_users[_uid] = itUid->second;
                return false;
            }
        }

        m_list_user_ptrs_.push_back(_ptr);

        if (_uid != 0)
        {
            m_map_id_users_[_uid] = std::prev(m_list_user_ptrs_.end());
        }

        m_map_conv_users[_conv] = std::prev(m_list_user_ptrs_.end());

        return true;
    }

    void UserManager::RemoveUserByConv(const NetId &_conv)
    {
        std::lock_guard<std::mutex> lock_(m_mutex_users_);

        auto it = m_map_conv_users.find(_conv);

        if (it == m_map_conv_users.end())
        {
            return;
        }

        UserId _uid = (*it->second)->userId();

        if (_uid != 0)
        {
            m_map_id_users_.erase(_uid);
        }

        m_list_user_ptrs_.erase(it->second);
        m_map_conv_users.erase(it);
    }

    void UserManager::RemoveOnlyConv(const NetId &_uid)
    {
        std::lock_guard<std::mutex> lock_(m_mutex_users_);

        auto it = m_map_conv_users.find(_uid);

        if (it == m_map_conv_users.end())
        {
            return;
        }

        if ((*it->second)->userId() == 0)
        {
            m_list_user_ptrs_.erase(it->second);
        }

        (*(it->second))->initConnectedState();
        m_map_conv_users.erase(it);
    }

}