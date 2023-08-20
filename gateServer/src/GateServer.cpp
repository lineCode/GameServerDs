#include "GateServer.h"
#include "../../common/network/EventManager.h"
#include "../../common/protobuf/cmdID.pb.h"
#include "../../common/protobuf/gate_game_msg.pb.h"
#include "../../common/protobuf/gate_login_msg.pb.h"
#include "../../common/protobuf/login_db_msg.pb.h"
#include "ClientService/ClientService.h"
#include "GameService/GameService.h"
#include "LoginService/LoginService.h"
#include "model/ConnectionManager.h"
#include "spdlog/spdlog.h"

namespace MiniProject
{

    GateServer *GateServer::instance_ = nullptr;
    std::mutex GateServer::instanceMutex_;

    void GateServer::onConnected(NetId _netID)
    {
        spdlog::info("Get Connection from {}", _netID);

        Connection::ConnectionPtr _ptr = std::make_shared<Connection>(_netID);

        ConnectionManager::Instance()->addConnectionPtr(_netID, _ptr);
    }

    void GateServer::onDisConnected(NetId _netID)
    {
        spdlog::info("DisConnect from {}", _netID);
        Connection::ConnectionPtr _ptr = ConnectionManager::Instance()->findConnectionByNetId(_netID);
        ConnectionManager::Instance()->removeConnectionPtr(_netID); // 处理策略上区分服务和客户节点，待处理
        // 在这个过程中收到的消息怎么办，待处理
        if (_ptr)
        {
            std::vector<int> net_ids = _ptr->GetConnectedNetIds();
            if (!net_ids.empty())
            {
                // gameserver断开后，将连接在当前gameserver上的客户下线
                for (int net_id : net_ids)
                {
                    std::cout << net_id << std::endl;
                    Connection::ConnectionPtr _cliptr = ConnectionManager::Instance()->findConnectionByNetId(net_id);
                    if (_cliptr)
                    {
                        _cliptr->set_endpoint(0, ConnectType::CT_DEFAULT);
                    }
                    force_disconnect(net_id);
                }
            }
            else if (_ptr->GetServerNetId() != 0)
            {
                GameService::NotifyUserDisConnect(_ptr->GetServerNetId(), _ptr->GetUserId());
                Connection::ConnectionPtr _serPtr = ConnectionManager::Instance()->findConnectionByNetId(_ptr->GetServerNetId());
                if (_serPtr)
                {
                    _serPtr->removeEnpoints(_netID);
                }
            }
        }
        // 服务节点断开连接，客户节点应该同步处理，保持一致性
    }

    void GateServer::initcallback()
    {
        // 客户侧的服务注册
        EventManager::Instance()->RegisterEventHandler(pb::CmdLoginCsAuthReq, ClientService::onClientLoginAuth);
        EventManager::Instance()->RegisterEventHandler(pb::CmdLoginCsReq, ClientService::onClientLogin);
        EventManager::Instance()->RegisterEventHandler(pb::CmdCreateRoomCsReq, ClientService::onClientCreateRoom);
        EventManager::Instance()->RegisterEventHandler(pb::CmdEnterRoomCsReq, ClientService::onClientEnterRoom);
        EventManager::Instance()->RegisterEventHandler(pb::CmdLeaveRoomCsReq, ClientService::onClientLeaveRoom);
        EventManager::Instance()->RegisterEventHandler(pb::CmdReadyInRoomCsReq, ClientService::onClientReadyRoom);
        EventManager::Instance()->RegisterEventHandler(pb::CmdChooseLevelCsReq, ClientService::onClientChooseLevel);

        // 游戏侧的服务注册
        EventManager::Instance()->RegisterEventHandler(pb::SS_GATE_MSG_LOGIN_RSP, GameService::onGameServerLogin);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GAME2GATE_ROOM_INFO_NOTIFY, GameService::onRoomMsgNotify);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GAME2GATE_BATTLE_INFO_NOTIFY, GameService::onBattlMsgNotify);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GAME_GATE_CREATE_ROOM_RSP, GameService::onGameCreateRoomRsp);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GAME_GATE_ENTER_ROOM_RSP, GameService::onGameEnterRoomRsp);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GAME_GATE_LEAVE_ROOM_RSQ, GameService::onGameLeaveRoomRsp);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GAME_GATE_USER_READY_RSP, GameService::onGameReadyRoomRsp);
        EventManager::Instance()->RegisterEventHandler(pb::SS_GAME_GATE_CHOOSE_LEVEL_RSP, GameService::onGameChooseLevelRsp);

        // 登陆侧的服务注册
        EventManager::Instance()->RegisterEventHandler(pb::SS_LOGIN_GATE_LOGINAUTH_RSP, LoginService::onLoginServerAuthRsp);
    }

}
