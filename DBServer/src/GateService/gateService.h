#ifndef MINIPROJECT_GATESERVICE_HPP
#define MINIPROJECT_GATESERVICE_HPP

#include "../../../common/network/EventCallBack.h"

namespace MiniProject
{
    class GateService
    {
    public:
        static void Register(const NetId &_netId, MessagePtr _ptr);

        static void LoginAuth(const NetId &_netId, MessagePtr _ptr);

    public:
        static void ResponseWithLoginAuth(const NetId &_netID, MsgPtr _ptr);
    };

} // namespace MiniProject

#endif