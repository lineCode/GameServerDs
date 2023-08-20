#ifndef MINIPROJECT_GAME_GATESERVICE_HPP
#define MINIPROJECT_GAME_GATESERVICE_HPP

#include "../../../common/network/EventCallBack.h"
#include "../model/User/User.h"

namespace MiniProject
{
    class GateService
    {
    public:
        static void onGateLogin(const NetId &_netId, MessagePtr _ptr);
        static void onUserDisConnect(const NetId &_netId, MessagePtr _ptr);

        static void onGateUserForceDisConnect(const NetId &_netId, MessagePtr _ptr);

        static void onGateCreateRoomReq(const NetId &_netId, MessagePtr _ptr);
        static void onGateEnterRoomReq(const NetId &_netId, MessagePtr _ptr);
        static void onGateLeaveRoomReq(const NetId &_netId, MessagePtr _ptr);
        static void onGateChooseLevelReq(const NetId &_netId, MessagePtr _ptr);
        static void onGateReadyInRoomReq(const NetId &_netId, MessagePtr _ptr);

    public:
        static void onUserUpdateNotify(const NetId &_netId, MessagePtr _ptr);

    public:
        static void ResponseWithGateLogin(const NetId &_netId, MsgPtr _ptr);

        static void onUserReconnectLogin(const NetId &_netId, User::UserPtr &_ptr);

        static void ResponseWithRoomMsgNotify(const UserId &_userid, MsgPtr _ptr);

        static void ResopnseWithBattleMsgNotify(const UserId &_userid, MsgPtr _ptr);

        static void ResponseWithCreateRoom(const NetId &_netId, MsgPtr _ptr);
        static void ResponseWithEnterRoom(const NetId &_netId, MsgPtr _ptr);
        static void ResponseWithLeaveRoom(const NetId &_netId, MsgPtr _ptr);
        static void ResponseWithChooseLevel(const NetId &_netId, MsgPtr _ptr);
        static void ResponseWithReadyRoom(const NetId &_netId, MsgPtr _ptr);
    };

} // namespace MiniProject

#endif