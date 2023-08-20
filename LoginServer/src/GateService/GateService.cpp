#include "GateService.h"
#include "spdlog/spdlog.h"
#include "../../../common/protobuf/gate_login_msg.pb.h"
#include "../../../common/protobuf/login_db_msg.pb.h"
#include <string>
#include "../LoginServer.h"

namespace MiniProject
{

    void GateService::onGateLoginAuthReq(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void onGateLoginAuthReq {}", _netId);

        pb::SSGate2LLoginAuthReq reqProxy;

        if (reqProxy.ParseFromString(*_ptr.get()))
        {
            uint32_t request_id = reqProxy.request_id();

            MsgPtr _dbreq(static_cast<Message *>(new pb::SSLogin2DLoginAuthReq()));

            ((pb::SSLogin2DLoginAuthReq *)(_dbreq.get()))->set_request_id(_netId);
            ((pb::SSLogin2DLoginAuthReq *)(_dbreq.get()))->set_allocated_req(new pb::SSGate2LLoginAuthReq(reqProxy));

            LoginServer::Instance()->send_message_by_type(ConnectType::CT_DB_SERVER, pb::SS_LOGIN_DB_AUTH_REQ, _dbreq);
        }
    }

}