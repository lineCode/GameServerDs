#include "Account.h"

namespace MiniProject
{

    Account::Account(const UserId &_uid, const UserName &_uname)
        : uID_(_uid), uName_(_uname)
    {
    }

    void Account::set_uid(const UserId &_uid)
    {
        uID_ = _uid;
    }

    void Account::set_username(const UserName &_name)
    {
        uName_ = _name;
    }

}