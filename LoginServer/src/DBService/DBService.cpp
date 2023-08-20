#include "DBService.h"
#include "spdlog/spdlog.h"
#include "../../../common/protobuf/login_db_msg.pb.h"
#include "../LoginServer.h"
namespace MiniProject
{

    void DBService::onLoginAuthRsp(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void DBService::onLoginAuthRsp {}", _netId);

        pb::SSDB2LLoginAuthRsp rspProxy;

        if (rspProxy.ParseFromString(*_ptr.get()))
        {
            uint32_t requestid = rspProxy.request_id();

            pb::SSLogin2GLoginAuthRsp *req = new pb::SSLogin2GLoginAuthRsp(rspProxy.rsp());

            MsgPtr _rspptr(static_cast<Message *>(req));

            LoginServer::Instance()->send_message_by_id(requestid, pb::SS_LOGIN_GATE_LOGINAUTH_RSP, _rspptr);
        }
    }
}