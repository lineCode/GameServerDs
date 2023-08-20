#ifndef MINIPROJECT_LOGINSERVER_GATESERVICE_HPP
#define MINIPROJECT_LOGINSERVER_GATESERVICE_HPP

#include "../../../common/network/EventCallBack.h"

namespace MiniProject
{

    class GateService
    {
    public:
        static void onGateLoginAuthReq(const NetId &_netId, MessagePtr _ptr);

        static void ResponseWithLoginAuth(const NetId &_netId, MsgPtr _ptr);
    };

}

#endif