#include "GameServer.h"
#include "../../../common/protobuf/CsCmd.pb.h"
#include "../../../common/protobuf/cmdID.pb.h"
#include "../../../common/protobuf/game_node_msg.pb.h"
#include "../../../common/protobuf/gate_game_msg.pb.h"
#include "../../common/network/EventManager.h"
#include "GateService/GateService.h"
#include "NodeService/NodeService.h"
#include "model/GameConnectionManager.h"
#include "spdlog/spdlog.h"

namespace MiniProject
{

    void GameServer::onConnected(NetId _netID)
    {
    }

    void GameServer::onDisConnected(NetId _netID)
    {
        Session::SessionPtr session = _managerPtr->findSessionByNetId(_netID);

        if (session)
        {
            if (session->GetNetType() == ConnectType::CT_GATE_SERVER)
            {
                onGateServerDisConnected(_netID);
            }
            else if (session->GetNetType() == ConnectType::CT_NODE_SERVER)
            {
                onNodeServerDisConnected(_netID);
            }
        }
    }

    void GameServer::initcallback()
    {
        // 初始化gate的服务
        EventManager::Instance()->RegisterEventHandler(pb::SS_GATE_MSG_LOGIN_REQ, GateService::onGateLogin);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GATE_GAME_USER_DISCONECT_NOTIFY, GateService::onUserDisConnect);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GATE_FORCE_USER_DISCONNECT_RSP, GateService::onGateUserForceDisConnect);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GATE_USER_INFO_NOTIFY, GateService::onUserUpdateNotify);

        EventManager::Instance()->RegisterEventHandler(pb::SS_GATE_GAME_CREATE_ROOM_REQ, GateService::onGateCreateRoomReq);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GATE_GAME_ENTER_ROOM_REQ, GateService::onGateCreateRoomReq);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GATE_GAME_LEAVE_ROOM_REQ, GateService::onGateLeaveRoomReq);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GATE_GAME_USER_READY_REQ, GateService::onGateReadyInRoomReq);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GATE_GAME_CHOOSE_LEVEL_REQ, GateService::onGateChooseLevelReq);

        // 初始化node服务
        EventManager::Instance()->RegisterEventHandler(pb::SS_GAME2GATE_ROOM_INFO_NOTIFY, NodeService::onRoomMsgInfoNotify);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GAME2GATE_BATTLE_INFO_NOTIFY, NodeService::onBattleMsgInfoNotify);

        EventManager::Instance()->RegisterEventHandler(pb::SS_NODE_GAME_CREATEROOM_RSP, NodeService::onNodeCreateRoomRsp);
        EventManager::Instance()->RegisterEventHandler(pb::SS_NODE_GAME_ENTERROOM_RSP, NodeService::onNodeEnterRoomRsp);
        EventManager::Instance()->RegisterEventHandler(pb::SS_NODE_GAME_LEAVEROOM_RSP, NodeService::onNodeLeaveRoomRsp);
        EventManager::Instance()->RegisterEventHandler(pb::SS_NODE_GAME_CHOOSELEVEL_RSP, NodeService::onNodeChooseLevelRsp);
        EventManager::Instance()->RegisterEventHandler(pb::SS_NODE_GAME_READYROOM_RSP, NodeService::onNodeReadyInRoomRsp);

        EventManager::Instance()->RegisterEventHandler(pb::SS_NODE_GAME_USER__DISCONNECT_TIMEOUT_NOTIFY, NodeService::onUserDisConnectTimeOutNotify);
    }

    uint64_t GameServer::GetAccountId(const UserId &_uid)
    {
        uint64_t serveruid = ((uint64_t)GetUid()) << 32;

        return (serveruid | ((uint64_t)_uid));
    }

    UserId GameServer::ParseIdFromIdProxy(uint64_t _userProxy)
    {
        return (UserId)(_userProxy & (UINT32_MAX));
    }

    void GameServer::onGateServerDisConnected(NetId _netId)
    {
        spdlog::info("void GameServer::onGateServerDisConnected(NetId _netId)");

        std::unordered_set<UserId> users = GameConnectionManager::Instance()->GetUsers(_netId);

        for (UserId user : users)
        {
            User::UserPtr uptr = UserManager::Instance()->findUserById(user);
            if (uptr)
            {
                uptr->set_connected(false);
                userDisConnected(uptr);
            }
        }
        GameConnectionManager::Instance()->removeNet(_netId);
    }

    void GameServer::onNodeServerDisConnected(NetId _netId)
    {
    }

    void GameServer::userDisConnected(User::UserPtr uptr)
    {
        if (uptr)
        {
            if (!uptr->isInRoom() && uptr->GetCurrentSt() == UserState::IDLE)
            {
                UserManager::Instance()->RemoveUserById(uptr->GetUserId());
                return;
            }
            if (uptr->isInRoom())
            {
                NodeService::NotifyUserDisConnect(uptr->GetUserId(), uptr->GetRoomId());
                // 若用户未在战斗关卡中，则直接将其提出房间
                if (!uptr->isBattle())
                {
                    UserManager::Instance()->RemoveUserById(uptr->GetUserId());
                }
                // 否则则等待用户重新链接，或游戏gameover时清除用户连接
            }
            else if (uptr->GetCurrentSt() == UserState::IN_PROCESS)
            {
                // 用户正在创建房间，不能够推出
                // NodeService::NotifyUserDisConnect(uptr->GetUserId(), uptr->GetRoomId());
            }
        }
    }

} // namespace MiniProject
