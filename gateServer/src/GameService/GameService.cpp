#include "GameService.h"

#include "../../../common/protobuf/CsCmd.pb.h"
#include "../../../common/protobuf/cmdID.pb.h"
#include "../../../common/protobuf/gate_game_msg.pb.h"
#include "../ClientService/ClientService.h"
#include "../GateServer.h"
#include "../model/Account/AccountManager.h"
#include "../model/ConnectionManager.h"
#include "spdlog/spdlog.h"
#include <cassert>

namespace MiniProject
{
    void GameService::onGameServerLogin(NetId _netId, MessagePtr _ptr)
    {
        spdlog::info("void GameService::onGameServerLogin() {}", _netId);

        pb::SSGateLoginRsp rsp;

        if (rsp.ParseFromString(*_ptr.get()))
        {
            uint32 req_id = rsp.request_id();
            Connection::ConnectionPtr _conptr = ConnectionManager::Instance()->findConnectionByNetId(req_id);
            if (_conptr)
            {
                _conptr->set_endpoint(_netId, ConnectType::CT_GAME_SERVER);
                _conptr->set_connected(true);
                _conptr->set_userid(rsp.rsp().accountid());
            }

            Connection::ConnectionPtr _serverPtr = ConnectionManager::Instance()->findConnectionByNetId(_netId);
            if (_serverPtr)
            {
                std::cout << _netId << "服务端口添加结点" << std::endl;
                _serverPtr->addEnpoints(req_id);
                _serverPtr->set_type(ConnectType::CT_GAME_SERVER);
            }
            MsgPtr _rspPtr(static_cast<Message *>(new pb::LoginScRsp(rsp.rsp())));
            ClientService::ResponseWithLogin(req_id, _rspPtr);
        }
    }

    void GameService::onRoomMsgNotify(NetId _netid, MessagePtr _ptr)
    {
        // spdlog::info("void GameService::onRoomMsgNotify() {}", _netid);

        // pb::SSGame2GRoomMsgNotify _notify;

        // if (_notify.ParseFromString(*_ptr.get()))
        // {
        //     UserId _uid = _notify.accountid();

        //     Connection::ConnectionPtr _uptr = ConnectionManager::Instance()->findConnectionByUserId(_uid);

        //     MsgPtr _rspptr(new pb::SCGate2ClientMsgNotify());
        //     ((pb::SCGate2ClientMsgNotify *)(_rspptr.get()))->set_accountid(_uid);

        //     if (_notify.has_roommsg())
        //     {
        //         pb::RoomInfoMsg *_roomMsg = new pb::RoomInfoMsg(_notify.roommsg());
        //         ((pb::SCGate2ClientMsgNotify *)(_rspptr.get()))->set_allocated_roommsg(_roomMsg);

        //         int size = _notify.users_size();

        //         for (int i = 0; i < size; ++i)
        //         {
        //             pb::InRoomUserInfo *_userinfo = ((pb::SCGate2ClientMsgNotify *)(_rspptr.get()))->add_users();
        //             _userinfo->CopyFrom(((pb::SCGate2ClientMsgNotify *)(_rspptr.get()))->users(i));
        //         }
        //     }

        //     GateServer::Instance()->send_message_by_id(_uptr->GetClientNetId(), pb::CmdNotifySCRoomMsg, _rspptr);
        // }
    }

    void GameService::onBattlMsgNotify(NetId _netid, MessagePtr _ptr)
    {
        // spdlog::info("void GameService::onBattlMsgNotify {}", _netid);

        // pb::SSGame2GBattleMsgNotify _notify;

        // if (_notify.ParseFromString(*_ptr.get()))
        // {
        //     UserId _uid = _notify.accountid();

        //     Connection::ConnectionPtr _uptr = ConnectionManager::Instance()->findConnectionByUserId(_uid);

        //     MsgPtr _rspptr(new pb::SCNode2ClientBattleMsgNotify());
        //     ((pb::SCNode2ClientBattleMsgNotify *)(_rspptr.get()))->set_accountid(_uid);
        //     ((pb::SCNode2ClientBattleMsgNotify *)(_rspptr.get()))->set_addr(_notify.addr());
        //     ((pb::SCNode2ClientBattleMsgNotify *)(_rspptr.get()))->set_port(_notify.port());
        //     ((pb::SCNode2ClientBattleMsgNotify *)(_rspptr.get()))->set_token(_notify.token());

        //     GateServer::Instance()->send_message_by_id(_uptr->GetClientNetId(), pb::CmdNotifySCBattleMsg, _rspptr);
        // }
    }

    void GameService::NotifyUserUpdate(Account::AccountPtr _ptr)
    {
        MsgPtr _rspptr(static_cast<Message *>(new pb::SSAccountInfoNotify()));

        pb::AccountInfo *_info = ((pb::SSAccountInfoNotify *)(_rspptr.get()))->add_infos();

        _info->set_acountid(_ptr->GetUID());
        _info->set_username(_ptr->GetUserName());

        GateServer::Instance()->send_message_by_type(ConnectType::CT_GAME_SERVER, pb::SS_GATE_MSG_CMD::SS_GATE_USER_INFO_NOTIFY, _rspptr);
    }

    void GameService::onGameCreateRoomRsp(NetId _netId, MessagePtr _ptr)
    {
        spdlog::info("void GameService::onGameCreateRoomRsp {}", _netId);

        pb::SSGame2GCreateRoomRsp rsp;

        if (rsp.ParseFromString(*_ptr.get()))
        {
            uint32_t request_id = rsp.request_id();

            // 是否需要每次更新服务器呢？
            //  Connection::ConnectionPtr _ptr = ConnectionManager::findConnectionByNetId(request_id);
            //  if (_ptr)
            //  {
            //      _ptr->set_endpoint(_netId, ConnectType::CT_GAME_SERVER);
            //  }

            MsgPtr _rspptr(static_cast<Message *>(new pb::TryCreateRoomScRsp(rsp.rsp())));
            GateServer::Instance()->send_message_by_id(request_id, pb::CmdCreateRoomScRsp, _rspptr);
        }
    }

    void GameService::onGameEnterRoomRsp(NetId _netId, MessagePtr _ptr)
    {
        spdlog::info("void GameService::onGameEnterRoomRsp {}", _netId);

        pb::SSGame2GEnterRoomRsp rsp;
        if (rsp.ParseFromString(*_ptr.get()))
        {
            uint32_t request_id = rsp.request_id();

            MsgPtr _rspptr(static_cast<Message *>(new pb::TryEnterRoomScRsp(rsp.rsp())));
            GateServer::Instance()->send_message_by_id(request_id, pb::CmdEnterRoomScRsp, _rspptr);
        }
    }
    void GameService::onGameLeaveRoomRsp(NetId _netId, MessagePtr _ptr)
    {
        spdlog::info("void GameService::onGameLeaveRoomRsp {}", _netId);

        pb::SSGame2GEnterRoomRsp rsp;
        if (rsp.ParseFromString(*_ptr.get()))
        {
            uint32_t request_id = rsp.request_id();

            MsgPtr _rspptr(static_cast<Message *>(new pb::TryEnterRoomScRsp(rsp.rsp())));
            GateServer::Instance()->send_message_by_id(request_id, pb::CmdEnterRoomScRsp, _rspptr);
        }
    }
    void GameService::onGameReadyRoomRsp(NetId _netId, MessagePtr _ptr)
    {
        spdlog::info("void GameService::onGameReadyRoomRsp {}", _netId);

        pb::SSGame2GReadyRoomRsp rsp;
        if (rsp.ParseFromString(*_ptr.get()))
        {
            uint32_t request_id = rsp.request_id();

            MsgPtr _rspptr(static_cast<Message *>(new pb::ReadyInRoomScRsp(rsp.rsp())));
            GateServer::Instance()->send_message_by_id(request_id, pb::CmdReadyInRoomScRsp, _rspptr);
        }
    }
    void GameService::onGameChooseLevelRsp(NetId _netId, MessagePtr _ptr)
    {
        spdlog::info("void GameService::onGameChooseLevelRsp {}", _netId);
        pb::SSGame2GChooseLevelRsp rsp;
        if (rsp.ParseFromString(*_ptr.get()))
        {
            uint32_t request_id = rsp.request_id();
            MsgPtr _rspptr(static_cast<Message *>(new pb::ChooseLevelScRsp(rsp.rsp())));
            GateServer::Instance()->send_message_by_id(request_id, pb::CmdChooseLevelScRsp, _rspptr);
        }
    }

    void GameService::NotifyUserDisConnect(NetId _netid, UserId _userid)
    {
        spdlog::info("User disconnected from the server {}", _netid);

        MsgPtr notify(static_cast<Message *>(new pb::SSGate2GUserDisConnectNotify()));

        ((pb::SSGate2GUserDisConnectNotify *)(notify.get()))->set_accountid((uint32_t)_userid);

        GateServer::Instance()->send_message_by_id(_netid, pb::SS_GATE_GAME_USER_DISCONECT_NOTIFY, notify);
    }

}