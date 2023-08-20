#ifndef MINIPROJECT_NODE_ROOMSERVICE_HPP
#define MINIPROJECT_NODE_ROOMSERVICE_HPP

#include "../../../common/network/EventCallBack.h"

namespace MiniProject
{
    class RoomService
    {
    public:
        static void onCreateRoomRsp(const NetId &_netid, MessagePtr _ptr);
        static void onEnterRoomRsp(const NetId &_netid, MessagePtr _ptr);
        static void onLeaveRoomRsp(const NetId &_netid, MessagePtr _ptr);
        static void onChooseLevelRsp(const NetId &_netid, MessagePtr _ptr);
        static void onUserReadyRsp(const NetId &_netid, MessagePtr _ptr);

        static void onUserDisConnectNotify(const NetId &_netid, MessagePtr _ptr);
    };

}

#endif