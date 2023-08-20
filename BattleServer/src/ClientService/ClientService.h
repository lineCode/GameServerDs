#ifndef MINIPROJECT_BS_CLIENTSERVICE_HPP
#define MINIPROJECT_BS_CLIENTSERVICE_HPP

#include "../../../common/network/EventCallBack.h"
#include "../model/Game/GameManager.h"

namespace MiniProject
{
    class ClientService
    {
    public:
        static void onClientInStruSync(const NetId &_netId, MessagePtr _ptr);
        static void onClientStateSync(const NetId &_netId, MessagePtr _ptr);

        static void onClientLinkGame(const NetId &_netId, MessagePtr _ptr);

        static void NotifyGameStateChange(const NetId &_netId, MsgPtr _ptr);
        static void SyncGameStatus(const NetId &_netId, MsgPtr _ptr);
        static void NotifyUserConnectStateChange(const NetId &_netId, MsgPtr _ptr); // connect/disconnect/join/leave

        static void forwardSalveInstruction(Game::GamePtr _gptr, MessagePtr _ptr);
        static void forwardHostState(Game::GamePtr _gptr, MessagePtr _ptr);
        static void forwardHostState(const NetId &netId, MessagePtr _ptr);

    public:
        static void ResponseWithLinkBattle(const NetId &_netId, MsgPtr _ptr);
    };

}

#endif