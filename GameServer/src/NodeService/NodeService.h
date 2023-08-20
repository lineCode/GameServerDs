#ifndef MINIPROJECT_GAME_NODESERVER
#define MINIPROJECT_GAME_NODESERVER
#include "../model/typedef.h"
#include "../../../common/network/EventCallBack.h"

namespace MiniProject
{
    class NodeService
    {
    public:
        static void onRoomMsgInfoNotify(const NetId &_netId, MessagePtr _ptr);
        static void onBattleMsgInfoNotify(const NetId &_netId, MessagePtr _ptr);

        static void onNodeCreateRoomRsp(const NetId &_netId, MessagePtr _ptr);
        static void onNodeEnterRoomRsp(const NetId &_netId, MessagePtr _ptr);
        static void onNodeLeaveRoomRsp(const NetId &_netId, MessagePtr _ptr);
        static void onNodeChooseLevelRsp(const NetId &_netId, MessagePtr _ptr);
        static void onNodeReadyInRoomRsp(const NetId &_netId, MessagePtr _ptr);

        static void onUserDisConnectTimeOutNotify(const NetId &_netId, MessagePtr _ptr);

    public:
        static void pullRoomMsgInfo(const UserId &_userId, const RoomId &_roomid, const NetId &_node = 0);

        static void pullBattleMsgInfo(const UserId &_userId, const RoomId &_roomid, const NetId &_node = 0);

        static void NotifyUserDisConnect(const UserId &_uid, const RoomId &_roomId);
    };

}

#endif