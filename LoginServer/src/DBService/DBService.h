#ifndef MINIPROJECT_LOGIN_DBSERVICE_HPP
#define MINIPROJECT_LOGIN_DBSERVICE_HPP

#include "../../../common/network/EventCallBack.h"

namespace MiniProject
{
    class DBService
    {
    public:
        static void onLoginAuthRsp(const NetId &_netId, MessagePtr _ptr);
    };

}

#endif