#ifndef MINIRPROJECT_ROOMSERVICE_HPP
#define MINIRPROJECT_ROOMSERVICE_HPP

#include "../../../common/network/EventCallBack.h"

namespace MiniProject
{
    class RoomService
    {
    public:
        static void onCreateGameRoom(const NetId &_netId, MessagePtr _ptr);
        static void onJoinGameRoom(const NetId &_netId, MessagePtr _ptr);
        static void onLeaveGameRoom(const NetId &_netId, MessagePtr _ptr);
    
    

        static void ResponseWithCreateGame(const NetId& _netId,MsgPtr _ptr);
        static void ResponseWithJoinGame(const NetId& _netId,MsgPtr _ptr);
        static void ResponseWithLeaveGame(const NetId& _netId,MsgPtr _ptr);
    
    };

}

#endif