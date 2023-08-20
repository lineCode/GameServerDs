#include "DBServer.h"
#include "GateService/gateService.h"
#include "../../common/network/EventManager.h"
#include "../../common/protobuf/login_db_msg.pb.h"

namespace MiniProject
{

    void DBServer::initcallback()
    {
        EventManager::Instance()->RegisterEventHandler(pb::SS_LOGIN_DB_AUTH_REQ, GateService::LoginAuth);
    }

    void DBServer::onConnected(NetId _netID)
    {
    }

    void DBServer::onDisConnected(NetId _netID)
    {
    }

}