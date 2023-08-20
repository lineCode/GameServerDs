#include "RoomServer.h"
#include "spdlog/spdlog.h"
#include "../../common/network/EventManager.h"
#include "NodeService/NodeService.h"

#include "../../../common/protobuf/node_room_msg.pb.h"

namespace MiniProject
{

    void RoomServer::onConnected(NetId _netID)
    {
        spdlog::info("Get Connection from {}", _netID);
    }

    void RoomServer::onDisConnected(NetId _netID)
    {
        spdlog::info("DisConnect from {}", _netID);
    }

    void RoomServer::initcallback()
    {
        EventManager::Instance()->RegisterEventHandler(pb::SS_NODE_ROOM_CREATEROOM_REQ, NodeService::onCreateRoom);
        EventManager::Instance()->RegisterEventHandler(pb::SS_NODE_ROOM_ENTERROOM_REQ, NodeService::onEnterRoom);
        EventManager::Instance()->RegisterEventHandler(pb::SS_NODE_ROOM_LEAVEROOM_REQ, NodeService::onLeaveRoom);
        EventManager::Instance()->RegisterEventHandler(pb::SS_NODE_ROOM_CHOOSELEVEL_REQ, NodeService::onChooseLevel);
        EventManager::Instance()->RegisterEventHandler(pb::SS_NODE_ROOM_READYROOM_REQ, NodeService::onUserReady);
        EventManager::Instance()->RegisterEventHandler(pb::SS_NODE_ROOM_CHOOSELEVEL_REQ, NodeService::onChooseLevel);

        

    }

}
