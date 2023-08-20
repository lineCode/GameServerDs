#ifndef MINIPROJECT_CLIENTSERVICE_HPP
#define MINIPROJECT_CLIENTSERVICE_HPP

#include "../../../common/network/EventCallBack.h"

namespace MiniProject
{
    class ClientService
    {
    public:
        static void onClientLoginAuth(const NetId &_netID, MessagePtr _ptr);

        static void onClientLogin(const NetId &_netID, MessagePtr _ptr);

        static void onClientCreateRoom(const NetId &_netID, MessagePtr _ptr);
        static void onClientEnterRoom(const NetId &_netID, MessagePtr _ptr);
        static void onClientLeaveRoom(const NetId &_netID, MessagePtr _ptr);
        static void onClientReadyRoom(const NetId &_netID, MessagePtr _ptr);
        static void onClientChooseLevel(const NetId &_netID, MessagePtr _ptr);

        static void onClientReEnterRoom(const NetId &_netID, MessagePtr _ptr);

    public:
        static void ResponseWithLoginAuth(const NetId &_netID, MsgPtr _ptr);

        static void ResponseWithLogin(const NetId &_netID, MsgPtr _ptr);

        static void ResponedWithServiceUnavailable(const NetId &_netID); //
    };

}

#endif