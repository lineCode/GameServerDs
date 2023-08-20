#include "AccountManager.h"
#include "spdlog/spdlog.h"

namespace MiniProject
{
    void AccountManager::addAccount(Account::AccountPtr _ptr)
    {
        std::lock_guard<std::mutex> lock(accountMutex_);

        UserName _uname = _ptr->GetUserName();
        UserId _uid = _ptr->GetUID();

        if (_uid == 0 && _uname == "")
        {
            return;
        }

        auto nameiter = accountMaps_.find(_uname);
        auto uiditer = userMaps_.find(_uid);

        if (nameiter != accountMaps_.end() && uiditer != userMaps_.end())
        {
            if (nameiter->second != uiditer->second)
            {
                spdlog::error("Account Manager sync error,exit");
                exit(0);
            }
            return;
        }
        else if (nameiter != accountMaps_.end())
        {
            userMaps_[_uid] = nameiter->second;
        }
        else if (uiditer != userMaps_.end())
        {
            accountMaps_[_uname] = uiditer->second;
        }
        else
        {
            accountLists_.push_back(_ptr);
            userMaps_[_uid] = std::prev(accountLists_.end());
            accountMaps_[_uname] = std::prev(accountLists_.end());
        }
    }

    void AccountManager::addAccount(const UserName &_name, Account::AccountPtr _ptr)
    {
        std::lock_guard<std::mutex> lock(accountMutex_);

        auto it = accountMaps_.find(_name);

        if (it != accountMaps_.end())
        {
            accountLists_.erase(it->second);
        }

        accountLists_.push_back(_ptr);
        accountMaps_[_name] = std::prev(accountLists_.end());
    }

    void AccountManager::updateUserId(Account::AccountPtr _ptr, const UserId &_uid)
    {
        std::lock_guard<std::mutex> lock(accountMutex_);

        UserId oldId = _ptr->GetUID();

        auto it = userMaps_.find(oldId);

        if (it != userMaps_.end())
        {
            userMaps_.erase(it);
        }

        UserName _uname = _ptr->GetUserName();

        auto _uiter = accountMaps_.find(_uname);

        if (_uiter == accountMaps_.end())
        {
            accountLists_.push_back(_ptr);
            accountMaps_[_uname] = std::prev(accountLists_.end());
        }

        userMaps_[_uid] = accountMaps_[_uname];
        _ptr->set_uid(_uid);
    }

    void AccountManager::removeAccount(const UserName &_name)
    {
        std::lock_guard<std::mutex> lock(accountMutex_);

        auto it = accountMaps_.find(_name);

        if (it != accountMaps_.end())
        {
            accountLists_.erase(it->second);
            accountMaps_.erase(it);
        }
    }

    Account::AccountPtr AccountManager::findAccountByUserId(const UserId &_uid)
    {
        std::lock_guard<std::mutex> lock(accountMutex_);

        auto it = userMaps_.find(_uid);

        if (it != userMaps_.end())
        {
            return *it->second;
        }

        return Account::AccountPtr();
    }

    Account::AccountPtr AccountManager::findAccountByUserName(const UserName &_name)
    {
        std::lock_guard<std::mutex> lock(accountMutex_);

        auto it = accountMaps_.find(_name);

        if (it != accountMaps_.end())
        {
            return *it->second;
        }
        return Account::AccountPtr();
    }

}