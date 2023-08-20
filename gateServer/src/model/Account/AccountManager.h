#ifndef MINIPROJECT_ACCOUNTMANAGER_HPP
#define MINIPROJECT_ACCOUNTMANAGER_HPP

#include <unordered_map>
#include <mutex>
#include <list>

#include "Account.h"
#include "../../../common/network/EventCallBack.h"

namespace MiniProject
{
    class AccountManager
    {
    public:
        static AccountManager *Instance()
        {
            static AccountManager manager;
            return &manager;
        }

        Account::AccountPtr findAccountByUserName(const UserName &_name);
        Account::AccountPtr findAccountByUserId(const UserId &_uid);


        void addAccount(Account::AccountPtr _ptr);
        void addAccount(const UserName &_name, Account::AccountPtr _ptr);
        void updateUserId(Account::AccountPtr _ptr, const UserId &_uid);
        void removeAccount(const UserName &_name);

    private:
        std::list<Account::AccountPtr> accountLists_;
        std::unordered_map<UserName, std::list<Account::AccountPtr>::iterator> accountMaps_;
        std::unordered_map<UserId, std::list<Account::AccountPtr>::iterator> userMaps_;
        std::mutex accountMutex_;
    };

} // namespace MiniProject

#endif