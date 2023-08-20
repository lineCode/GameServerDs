#include "gateService.h"
#include "spdlog/spdlog.h"
#include "../../../common/protobuf/login_db_msg.pb.h"
#include "../DBHandle/dbHandle.h"
#include "../DBServer.h"

namespace MiniProject
{

    void GateService::Register(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("GateService::Register({})", _netId);
    }

    void GateService::LoginAuth(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("GateService::LoginAuth({})", _netId);

        pb::SSLogin2DLoginAuthReq reqProxy;

        if (reqProxy.ParseFromString(*_ptr.get()))
        {
            pb::LoginAuthCsReq req = reqProxy.req().req();

            std::string username = req.username();
            std::string passwd = req.passwd();

            MsgPtr _rsp(static_cast<Message *>(new pb::SSDB2LLoginAuthRsp()));
            ((pb::SSDB2LLoginAuthRsp *)(_rsp.get()))->set_request_id(reqProxy.request_id());

            pb::SSLogin2GLoginAuthRsp *loginRsp = new pb::SSLogin2GLoginAuthRsp();
            loginRsp->set_request_id(reqProxy.req().request_id());

            pb::LoginAuthScRsp *gateRsp = new pb::LoginAuthScRsp;
            loginRsp->set_allocated_rsp(gateRsp);
            ((pb::SSDB2LLoginAuthRsp *)(_rsp.get()))->set_allocated_rsp(loginRsp);

            gateRsp->set_username(username);
            std::string _pass;
            uint32_t _uid = DbHandle::GetVerification(username, _pass);

            if (_uid <= 0 && !DbHandle::RegisterUser(username, passwd))
            {
                spdlog::error("failed to create user:{}", username);
                gateRsp->set_retcode(pb::LoginAuthScRsp::RET_FAILED);
                ResponseWithLoginAuth(_netId, _rsp);
                return;
            }

            if (_uid <= 0)
            {
                _uid = DbHandle::GetVerification(username, _pass);
            }

            if (_uid <= 0)
            {
                spdlog::error("failed to create user:{}", username);
                gateRsp->set_retcode(pb::LoginAuthScRsp::RET_FAILED);
                ResponseWithLoginAuth(_netId, _rsp);
                return;
            }

            if (passwd == _pass)
            {
                gateRsp->set_retcode(pb::LoginAuthScRsp::RET_SUCC);
                gateRsp->set_accountid(_uid);
            }
            else
            {
                gateRsp->set_retcode(pb::LoginAuthScRsp::RET_FAILED);
            }
            ResponseWithLoginAuth(_netId, _rsp);
            return;
        }
    }

    void GateService::ResponseWithLoginAuth(const NetId &_netID, MsgPtr _ptr)
    {
        DBServer::Instance()->send_message_by_id(_netID, pb::SS_DB_LOGIN_AUTH_RSP, _ptr);
    }

}