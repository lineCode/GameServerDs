#include "NodeServer.h"
#include "spdlog/spdlog.h"
#include "GameService/GameService.h"
#include "RoomService/RoomService.h"
#include "../../../common/network/EventManager.h"

#include "../../../common/protobuf/game_node_msg.pb.h"
#include "../../../common/protobuf/node_room_msg.pb.h"

namespace MiniProject
{

    void NodeServer::onConnected(NetId _netID)
    {
        spdlog::info("Get Connection from {}", _netID);
    }

    void NodeServer::onDisConnected(NetId _netID)
    {
        spdlog::info("DisConnect from {}", _netID);
    }

    void NodeServer::initcallback()
    {

        // 注册gameservice的回调函数
        EventManager::Instance()->RegisterEventHandler(pb::SS_GAME_NODE_MSG_CMD::SS_GAME_NODE_CREATEROOM_REQ, GameService::onGameCreateRoomReq);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GAME_NODE_MSG_CMD::SS_GAME_NODE_ENTERROOM_REQ, GameService::onGameEnterRoomReq);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GAME_NODE_MSG_CMD::SS_GAME_NODE_LEAVEROOM_REQ, GameService::onGameLeaveRoomReq);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GAME_NODE_MSG_CMD::SS_GAME_NODE_CHOOSELEVEL_REQ, GameService::onGameChooseLevelReq);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GAME_NODE_MSG_CMD::SS_GAME_NODE_READYROOM_REQ, GameService::onGameUserReadyReq);

        // 注册roomservice的回调函数
        EventManager::Instance()->RegisterEventHandler(pb::SS_NODE_ROOM_MSG_CMD::SS_ROOM_NODE_CREATEROOM_RSP, RoomService::onCreateRoomRsp);
        EventManager::Instance()->RegisterEventHandler(pb::SS_NODE_ROOM_MSG_CMD::SS_ROOM_NODE_ENTERROOM_RSP, RoomService::onEnterRoomRsp);
        EventManager::Instance()->RegisterEventHandler(pb::SS_NODE_ROOM_MSG_CMD::SS_ROOM_NODE_LEAVEROOM_RSP, RoomService::onLeaveRoomRsp);
        EventManager::Instance()->RegisterEventHandler(pb::SS_NODE_ROOM_MSG_CMD::SS_ROOM_NODE_CHOOSELEVEL_RSP, RoomService::onChooseLevelRsp);
        EventManager::Instance()->RegisterEventHandler(pb::SS_NODE_ROOM_MSG_CMD::SS_ROOM_NODE_READYROOM_RSP, RoomService::onUserReadyRsp);
    }

}
