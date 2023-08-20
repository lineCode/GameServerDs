#ifndef MINIPROJECT_LOGINSERVICE_HPP
#define MINIPROJECT_LOGINSERVICE_HPP

#include "../../../common/network/EventCallBack.h"

namespace MiniProject
{
    class LoginService
    {
    public:
        static void onLoginServerAuthRsp(const NetId &_netID, MessagePtr _ptr);
    };

}

#endif