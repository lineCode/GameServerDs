#ifndef MINIPROJECT_ROOM_NODESERVICE_HPP
#define MINIPROJECT_ROOM_NODESERVICE_HPP

#include "../../../common/network/EventCallBack.h"
#include "../model/Room/RoomManager.h"

namespace MiniProject
{
    class NodeService
    {
    public:
        static void onCreateRoom(const NetId &_netid, MessagePtr _ptr);
        static void onEnterRoom(const NetId &_netid, MessagePtr _ptr);
        static void onLeaveRoom(const NetId &_netid, MessagePtr _ptr);
        static void onUserReady(const NetId &_netid, MessagePtr _ptr);
        static void onChooseLevel(const NetId &_netid, MessagePtr _ptr);

        static void onDisConnectRoom(const NetId &_netid, MessagePtr _ptr);
        static void onReConnectRoom(const NetId &_netid, MessagePtr _ptr);

    public:
        static void ResponseWithCreateRoom(const NetId &_netid, MsgPtr _ptr);
        static void ResponseWithEnterRoom(const NetId &_netid, MsgPtr _ptr);
        static void ResponseWithLeaveRoom(const NetId &_netid, MsgPtr _ptr);
        static void ResponseWithUserReady(const NetId &_netid, MsgPtr _ptr);
        static void ResponseWithChooseLevel(const NetId &_netid, MsgPtr _ptr);

    public:
        static void NotifyRoomTotalUsers(Room::RoomPtr _ptr);
        static void NotifyGameStart(Room::RoomPtr _ptr);
    };

} // namespace MiniProject

#endif