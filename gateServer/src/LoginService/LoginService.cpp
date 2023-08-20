#include "LoginService.h"
#include "spdlog/spdlog.h"
#include "../../../common/protobuf/gate_login_msg.pb.h"
#include "../../../common/protobuf/CsCmd.pb.h"
#include "../../../common/protobuf/cmdID.pb.h"
#include "../GateServer.h"
#include "../GameService/GameService.h"
#include "../model/Account/AccountManager.h"

namespace MiniProject
{
    void LoginService::onLoginServerAuthRsp(const NetId &_netID, MessagePtr _ptr)
    {
        spdlog::info("LoginService::onLoginLoginAuth {}", _netID);

        // pb::SSLoginAuthRsp rsp;
        pb::SSLogin2GLoginAuthRsp rspProxy;

        if (rspProxy.ParseFromString(*_ptr.get()))
        {
            uint32_t request_id = rspProxy.request_id();

            MsgPtr _rspPtr(static_cast<Message *>(new pb::LoginAuthScRsp(rspProxy.rsp())));

            pb::LoginAuthScRsp rsp = rspProxy.rsp();

            if (rsp.retcode() == pb::LoginAuthScRsp::RET_SUCC)
            {
                UserName _name = rsp.username();
                UserId _uid = rsp.accountid();
                spdlog::info("{} ***** {}", _name, _uid);
                Account::AccountPtr _accptr = std::make_shared<Account>(_uid, _name);
                GameService::NotifyUserUpdate(_accptr);
            }
            GateServer::Instance()->send_message_by_id(request_id, pb::CmdID::CmdLoginScAuthRsp, _rspPtr);
        }
    }

}