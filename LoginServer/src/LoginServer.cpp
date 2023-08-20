#include "LoginServer.h"
#include "spdlog/spdlog.h"

#include "GateService/GateService.h"
#include "DBService/DBService.h"
#include "../../common/network/EventManager.h"
#include "../../common/protobuf/login_db_msg.pb.h"
#include "../../common/protobuf/gate_login_msg.pb.h"

namespace MiniProject
{

    void LoginServer::onConnected(NetId _netID)
    {
        spdlog::info("Get Connection from {}", _netID);
    }

    void LoginServer::onDisConnected(NetId _netID)
    {
        spdlog::info("DisConnect from {}", _netID);
    }

    void LoginServer::initcallback()
    {
        // 注册gateservice的服务
        EventManager::Instance()->RegisterEventHandler(pb::SS_GATE_LOGIN_LOGINAUTH_REQ, GateService::onGateLoginAuthReq);

        // 注册DBService的服务
        EventManager::Instance()->RegisterEventHandler(pb::SS_DB_LOGIN_AUTH_RSP, DBService::onLoginAuthRsp);
    }

}
