#ifndef MINIPROJECT_GAMESERVICE_HPP
#define MINIPROJECT_GAMESERVICE_HPP

#include "../../../common/network/EventCallBack.h"
#include "../model/Account/AccountManager.h"

namespace MiniProject
{
    class GameService
    {
    public:
        static void onGameServerLogin(NetId _netId, MessagePtr _ptr);

        static void onRoomMsgNotify(NetId _netId, MessagePtr _ptr);
        static void onBattlMsgNotify(NetId _netId, MessagePtr _ptr);

        static void onGameCreateRoomRsp(NetId _netId, MessagePtr _ptr);
        static void onGameEnterRoomRsp(NetId _netId, MessagePtr _ptr);
        static void onGameLeaveRoomRsp(NetId _netId, MessagePtr _ptr);
        static void onGameReadyRoomRsp(NetId _netId, MessagePtr _ptr);
        static void onGameChooseLevelRsp(NetId _netId, MessagePtr _ptr);

    public:
        static void NotifyUserUpdate(Account::AccountPtr _ptr);
        static void NotifyUserDisConnect(NetId _netid,UserId _userId);
    };

} // namespace MiniProject

#endif