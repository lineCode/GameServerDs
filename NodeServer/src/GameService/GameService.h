#ifndef MINIPROJECT_NODE_GAMESERVICE_HPP
#define MINIPROJECT_NODE_GAMESERVICE_HPP

#include "../../../common/network/EventCallBack.h"

namespace MiniProject
{
    class GameService
    {
    public:
        static void onGameEnterRoomReq(const NetId &_netid, MessagePtr _ptr);
        static void onGameCreateRoomReq(const NetId &_netid, MessagePtr _ptr);
        static void onGameLeaveRoomReq(const NetId &_netid, MessagePtr _ptr);
        static void onGameChooseLevelReq(const NetId &_netid, MessagePtr _ptr);
        static void onGameUserReadyReq(const NetId &_netid, MessagePtr _ptr);

        static void onPullRoomMsgReq(const NetId &_netid, MessagePtr _ptr);
        static void onPullBattleMsgReq(const NetId &_netid, MessagePtr _ptr);

    public:
        static void onGameUserDisConnect(const NetId &_netid, MessagePtr _ptr);
        static void onGameUserReConnect(const NetId &_netid, MessagePtr _ptr);

    public:
        static void ResponseWithGameCreateRoom(const NetId &_netId, MsgPtr _ptr);
        static void ResponseWithGameEnterRoom(const NetId &_netId, MsgPtr _ptr);
        static void ResponseWithGameLeaveRoom(const NetId &_netId, MsgPtr _ptr);
        static void ResponseWithGameUserReady(const NetId &_netId, MsgPtr _ptr);
        static void ResponseWithGameChooseLevel(const NetId &_netId, MsgPtr _ptr);
    };
}

#endif