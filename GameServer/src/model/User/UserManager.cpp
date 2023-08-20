#include "UserManager.h"

namespace MiniProject
{
    bool UserManager::addUserById(User::UserPtr _ptr)
    {
        std::lock_guard<std::mutex> lock(userMutex_);

        auto it = userMaps_.find(_ptr->GetUserId());

        if (it != userMaps_.end())
        {
            return false;
        }
        userMaps_[_ptr->GetUserId()] = _ptr;
        return true;
    }

    void UserManager::RemoveUserById(const UserId &_uid)
    {
        std::lock_guard<std::mutex> lock(userMutex_);

        auto it = userMaps_.find(_uid);

        if (it != userMaps_.end())
        {
            userMaps_.erase(it);
        }
    }

    User::UserPtr UserManager::findUserById(const UserId &_uid)
    {
        std::lock_guard<std::mutex> lock_(userMutex_);

        auto it = userMaps_.find(_uid);

        if (it == userMaps_.end())
        {
            return User::UserPtr();
        }

        return it->second;
    }

}