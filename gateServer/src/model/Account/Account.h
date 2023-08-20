#ifndef MINIPROJCET_ACCOUNT_HPP
#define MINIPROJCET_ACCOUNT_HPP

#include "../structDef.h"
#include <memory>

namespace MiniProject
{
    class Account
    {
    public:
        typedef std::shared_ptr<Account> AccountPtr;

    public:
        Account(const UserId &_uid, const UserName &_uname);

        void set_uid(const UserId &_uid);
        void set_username(const UserName &_uname);

        UserId GetUID()
        {
            return uID_;
        }

        UserName GetUserName()
        {
            return uName_;
        }

    private:
        UserId uID_ = 0;
        UserName uName_ = "";
    };

}

#endif