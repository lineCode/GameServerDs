#include "ClientService.h"
#include "../../../common/protobuf/CsCmd.pb.h"
#include "../../../common/protobuf/cmdID.pb.h"
#include "../../../common/protobuf/gate_game_msg.pb.h"
#include "../../../common/protobuf/gate_login_msg.pb.h"
#include "../GameService/GameService.h"
#include "../GateServer.h"
#include "../model/Account/AccountManager.h"
#include "../model/ConnectionManager.h"
#include "spdlog/spdlog.h"

namespace MiniProject
{
    void ClientService::onClientLoginAuth(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("onClientLoginAuth : {}", _netId);

        pb::LoginAuthCsReq req;

        if (req.ParseFromString(*_ptr.get()))
        {
            MsgPtr _reqptr(static_cast<Message *>(new pb::SSGate2LLoginAuthReq()));

            ((pb::SSGate2LLoginAuthReq *)(_reqptr.get()))->set_request_id(_netId);

            pb::LoginAuthCsReq *csreqptr = new pb::LoginAuthCsReq(req);

            ((pb::SSGate2LLoginAuthReq *)(_reqptr.get()))->set_allocated_req(csreqptr);

            GateServer::Instance()->send_message_by_type(ConnectType::CT_LOGIN_SERVER, pb::SS_GATE_LOGIN_LOGINAUTH_REQ, _reqptr);
        }
    }

    void ClientService::onClientLogin(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("onClientLogin : {}", _netId);

        pb::LoginCsReq req;

        if (req.ParseFromString(*_ptr.get()))
        {

            MsgPtr _ptr(static_cast<Message *>(new pb::SSGateLoginReq));
            ((pb::SSGateLoginReq *)_ptr.get())->set_requsted_id(_netId);
            ((pb::SSGateLoginReq *)_ptr.get())->set_allocated_req(new pb::LoginCsReq(req));

            // 通过redis去缓冲用户的登录数据，将用户的数据发送到某个服务器上，防止重新登录
            Connection::ConnectionPtr _cptr = ConnectionManager::Instance()->findConnectionByNetId(_netId);
            uint32_t _serverid = _cptr->GetServerNetId();
            if (_serverid != 0)
            {
                GateServer::Instance()->send_message_by_id(_serverid, pb::SS_GATE_MSG_LOGIN_REQ, _ptr);
            }
            else
            {
                GateServer::Instance()->send_message_by_type(ConnectType::CT_GAME_SERVER, pb::SS_GATE_MSG_LOGIN_REQ, _ptr);
            }
        }
    }

    void ClientService::ResponseWithLoginAuth(const NetId &_netId, MsgPtr _ptr)
    {
        GateServer::Instance()->send_message_by_id(_netId, pb::CmdID::CmdLoginScAuthRsp, _ptr);
    }

    void ClientService::ResponseWithLogin(const NetId &_netID, MsgPtr _ptr)
    {
        GateServer::Instance()->send_message_by_id(_netID, pb::CmdID::CmdLoginScRsp, _ptr);
    }

    void ClientService::onClientCreateRoom(const NetId &_netID, MessagePtr _ptr)
    {
        spdlog::info("void ClientService::onClientCreateRoom {}", _netID);

        pb::TryCreateRoomCsReq req;

        if (req.ParseFromString(*_ptr.get()))
        {
            MsgPtr reqptr(static_cast<Message *>(new pb::SSGate2GCreateRoomReq()));
            ((pb::SSGate2GCreateRoomReq *)(reqptr.get()))->set_request_id(_netID);
            pb::TryCreateRoomCsReq *_reqptr = new pb::TryCreateRoomCsReq(req);
            ((pb::SSGate2GCreateRoomReq *)(reqptr.get()))->set_allocated_req(_reqptr);
            Connection::ConnectionPtr cptr = ConnectionManager::Instance()->findConnectionByNetId(_netID);
            if (!GateServer::Instance()->send_message_by_id(cptr->GetServerNetId(), pb::SS_GATE_GAME_CREATE_ROOM_REQ, reqptr))
            {
                ResponedWithServiceUnavailable(_netID);
            }
        }
    }

    void ClientService::onClientEnterRoom(const NetId &_netID, MessagePtr _ptr)
    {
        spdlog::info("void ClientService::onClientEnterRoom {}", _netID);

        pb::TryEnterRoomCsReq req;

        if (req.ParseFromString(*_ptr.get()))
        {
            MsgPtr reqptr(static_cast<Message *>(new pb::SSGate2GEnterRoomReq()));
            ((pb::SSGate2GEnterRoomReq *)(reqptr.get()))->set_request_id(_netID);

            pb::TryEnterRoomCsReq *_rrptr = new pb::TryEnterRoomCsReq(req);
            ((pb::SSGate2GEnterRoomReq *)(reqptr.get()))->set_allocated_req(_rrptr);

            Connection::ConnectionPtr cptr = ConnectionManager::Instance()->findConnectionByNetId(_netID);
            if (!GateServer::Instance()->send_message_by_id(cptr->GetServerNetId(), pb::SS_GATE_GAME_ENTER_ROOM_REQ, reqptr))
            {
                ResponedWithServiceUnavailable(_netID);
            }
        }
    }

    void ClientService::onClientLeaveRoom(const NetId &_netID, MessagePtr _ptr)
    {
        spdlog::info("void ClientService::onClientLeaveRoom {}", _netID);

        pb::TryLeaveRoomCsReq req;

        if (req.ParseFromString(*_ptr.get()))
        {
            MsgPtr reqptr(static_cast<Message *>(new pb::SSGate2GLeaveRoomReq()));
            ((pb::SSGate2GLeaveRoomReq *)(reqptr.get()))->set_request_id(_netID);

            pb::TryLeaveRoomCsReq *_rrptr = new pb::TryLeaveRoomCsReq(req);
            ((pb::SSGate2GLeaveRoomReq *)(reqptr.get()))->set_allocated_req(_rrptr);

            Connection::ConnectionPtr cptr = ConnectionManager::Instance()->findConnectionByNetId(_netID);
            if (!GateServer::Instance()->send_message_by_id(cptr->GetServerNetId(), pb::SS_GATE_GAME_LEAVE_ROOM_REQ, reqptr))
            {
                ResponedWithServiceUnavailable(_netID);
            }
        }
    }

    void ClientService::onClientReadyRoom(const NetId &_netID, MessagePtr _ptr)
    {
        spdlog::info("void ClientService::onClientReadyRoom {}", _netID);

        pb::ReadyInRoomCsReq req;

        if (req.ParseFromString(*_ptr.get()))
        {
            MsgPtr reqptr(static_cast<Message *>(new pb::SSGate2GReadyRoomReq()));
            ((pb::SSGate2GReadyRoomReq *)(reqptr.get()))->set_request_id(_netID);

            pb::ReadyInRoomCsReq *_rrptr = new pb::ReadyInRoomCsReq(req);
            ((pb::SSGate2GReadyRoomReq *)(reqptr.get()))->set_allocated_req(_rrptr);

            Connection::ConnectionPtr cptr = ConnectionManager::Instance()->findConnectionByNetId(_netID);
            if (!GateServer::Instance()->send_message_by_id(cptr->GetServerNetId(), pb::SS_GATE_GAME_USER_READY_REQ, reqptr))
            {
                ResponedWithServiceUnavailable(_netID);
            }
        }
    }

    void ClientService::onClientChooseLevel(const NetId &_netID, MessagePtr _ptr)
    {
        spdlog::info("void ClientService::onClientChooseLevel {}", _netID);

        pb::ChooseLevelCsReq req;

        if (req.ParseFromString(*_ptr.get()))
        {
            MsgPtr reqptr(static_cast<Message *>(new pb::SSGate2GChooseLevelReq()));
            ((pb::SSGate2GChooseLevelReq *)(reqptr.get()))->set_request_id(_netID);

            pb::ChooseLevelCsReq *_rrptr = new pb::ChooseLevelCsReq(req);
            ((pb::SSGate2GChooseLevelReq *)(reqptr.get()))->set_allocated_req(_rrptr);

            Connection::ConnectionPtr cptr = ConnectionManager::Instance()->findConnectionByNetId(_netID);
            if (!GateServer::Instance()->send_message_by_id(cptr->GetServerNetId(), pb::SS_GATE_GAME_USER_READY_REQ, reqptr))
            {
                ResponedWithServiceUnavailable(_netID);
            }
        }
    }

    void ClientService::ResponedWithServiceUnavailable(const NetId &_netid)
    {
    }

}