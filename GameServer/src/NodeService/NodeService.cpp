#include "NodeService.h"
#include "spdlog/spdlog.h"
#include "../GameServer.h"
#include "../GateService/GateService.h"
#include "../../../common/protobuf/gate_game_msg.pb.h"
#include "../../../common/network/EventCallBack.h"
#include "../../../common/protobuf/game_node_msg.pb.h"
#include "../model/User/UserManager.h"
#include <cassert>

namespace MiniProject
{
    void NodeService::onRoomMsgInfoNotify(const NetId &_netId, MessagePtr _ptr)
    {
        // spdlog::info("onRoomMsgInfo {}", _netId);

        // pb::SSN2GPullMsgInfoRsp rsp;

        // if (rsp.ParseFromString(*_ptr.get()))
        // {
        //     uint32_t _repid = rsp.accountid();
        //     pb::SSN2GPullMsgInfoRsp::RetCode _ret = rsp.retcode();

        //     MsgPtr _rspptr(static_cast<Message *>(new pb::SSGame2GRoomMsgNotify()));
        //     ((pb::SSGame2GRoomMsgNotify *)(_rspptr.get()))->set_accountid(rsp.accountid());

        //     if (_ret == pb::SSN2GPullMsgInfoRsp::RET_SUCC)
        //     {
        //         int user_size = rsp.userlists_size();
        //         for (int i = 0; i < user_size; ++i)
        //         {
        //             pb::InRoomUserInfo *userinfo = ((pb::SSGame2GRoomMsgNotify *)(_rspptr.get()))->add_users();
        //             *userinfo = rsp.userlists(i);
        //             // userinfo->set_accountid(rsp.userlists[i].accountid());
        //             // userinfo->set_playerid(rsp.userlists[i].playerid());
        //             // userinfo->set_ishost(rsp.userlists[i].ishost());
        //             // userinfo->set_readystate(rsp.userlists[i].readystate());
        //             // userinfo->set_name(rsp.userlists[i].name());
        //         }
        //         if (rsp.has_roommsg())
        //         {
        //             pb::RoomInfoMsg roomInfoMsg = rsp.roommsg();
        //             ((pb::SSGame2GRoomMsgNotify *)(_rspptr.get()))->set_allocated_roommsg(&roomInfoMsg);
        //         }
        //     }
        //     // 拉取失败是否有额外处理呢？
        //     GateService::ResponseWithRoomMsgNotify(rsp.accountid(), _rspptr);
        // }
    }

    void NodeService::onBattleMsgInfoNotify(const NetId &_netId, MessagePtr _ptr)
    {
        // spdlog::info("onBattleMsgInfoNotify {}", _netId);

        // pb::SSN2GPullBattleMsgInfoRsp rsp;

        // if (rsp.ParseFromString(*_ptr.get()))
        // {
        //     uint32_t accid = rsp.accountid();

        //     MsgPtr _rspptr(new pb::SSGame2GBattleMsgNotify());
        //     ((pb::SSGame2GBattleMsgNotify *)(_rspptr.get()))->set_accountid(accid);

        //     if (rsp.retcode() == pb::SSN2GPullBattleMsgInfoRsp::RET_SUCC && rsp.has_battleservermsg())
        //     {
        //         ((pb::SSGame2GBattleMsgNotify *)(_rspptr.get()))->set_addr(rsp.battleservermsg().addr());
        //         ((pb::SSGame2GBattleMsgNotify *)(_rspptr.get()))->set_port(rsp.battleservermsg().port());
        //         ((pb::SSGame2GBattleMsgNotify *)(_rspptr.get()))->set_token(rsp.battleservermsg().token());
        //     }

        //     GateService::ResopnseWithBattleMsgNotify(rsp.accountid(), _rspptr);
        // }
    }

    void NodeService::pullRoomMsgInfo(const UserId &_userId, const RoomId &_roomid, const NetId &node)
    {
        spdlog::info("PullRoomMsgInfo {}", _roomid);

        MsgPtr _ptr(new pb::SSG2NPullMsgInfoReq);

        ((pb::SSG2NPullMsgInfoReq *)(_ptr.get()))->set_accountid(_userId);
        ((pb::SSG2NPullMsgInfoReq *)(_ptr.get()))->set_roomid(_roomid);

        if (node == 0)
        {
            GameServer::Instance()->send_message_by_type(ConnectType::CT_NODE_SERVER, pb::SS_GAME_NODE_PULLROOMINFO_REQ, _ptr);
        }
        else
        {
            GameServer::Instance()->send_message_by_id(node, pb::SS_GAME_NODE_PULLROOMINFO_REQ, _ptr);
        }
    }

    void NodeService::pullBattleMsgInfo(const UserId &_userId, const RoomId &_roomid, const NetId &node)
    {
        spdlog::info("PullRoomMsgInfo {}", _roomid);

        MsgPtr _ptr(new pb::SSG2NPullMsgInfoReq);

        ((pb::SSG2NPullMsgInfoReq *)(_ptr.get()))->set_accountid(_userId);
        ((pb::SSG2NPullMsgInfoReq *)(_ptr.get()))->set_roomid(_roomid);

        if (node == 0)
        {
            GameServer::Instance()->send_message_by_type(ConnectType::CT_NODE_SERVER, pb::SS_GAME_NODE_PULLBATTLEINFO_REQ, _ptr);
        }
        else
        {
            GameServer::Instance()->send_message_by_id(node, pb::SS_GAME_NODE_PULLBATTLEINFO_REQ, _ptr);
        }
    }

    void NodeService::onNodeCreateRoomRsp(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void NodeService::onNodeCreateRoomRsp {}", _netId);

        pb::SSN2GCreateRoomRsp rspProxy;

        if (rspProxy.ParseFromString(*_ptr.get()))
        {
            pb::TryCreateRoomScRsp rsp = rspProxy.rsp();

            uint64_t accountIdProxy = rspProxy.accountid();

            UserId accountId = GameServer::Instance()->ParseIdFromIdProxy(accountIdProxy);

            User::UserPtr _uPtr = UserManager::Instance()->findUserById(accountId);

            if (rsp.retcode() == pb::TryCreateRoomScRsp::RET_SUCC)
            {
                assert(_uPtr.get() != nullptr);

                _uPtr->setRoomId(rsp.roomid());

                _uPtr->setServerNetId(_netId);

                // 房间信息仅保留是否在房间内和房间的id号

                if (!_uPtr->isConnected())
                {
                    NodeService::NotifyUserDisConnect(accountId, _uPtr->GetRoomId());
                    return;
                }
            }

            if (_uPtr->isConnected())
            {
                MsgPtr _rspptr(static_cast<Message *>(new pb::SSGame2GCreateRoomRsp()));
                ((pb::SSGame2GCreateRoomRsp *)(_rspptr.get()))->set_request_id(_uPtr->GetGateNetId());
                pb::TryCreateRoomScRsp *scrsp = new pb::TryCreateRoomScRsp(rspProxy.rsp());
                ((pb::SSGame2GCreateRoomRsp *)(_rspptr.get()))->set_allocated_rsp(scrsp);
                GateService::ResponseWithCreateRoom(_uPtr->GetClientNetId(), _rspptr);
            }
            _uPtr->setCurrentSt(UserState::IDLE);
        }
    }

    void NodeService::onNodeEnterRoomRsp(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void NodeService::onNodeEnterRoomRsp {}", _netId);

        pb::SSN2GEnterRoomRsp rspProxy;

        if (rspProxy.ParseFromString(*_ptr.get()))
        {
            pb::TryEnterRoomScRsp rsp = rspProxy.rsp();

            uint64_t accountIdProxy = rspProxy.accountid();

            UserId accountId = GameServer::Instance()->ParseIdFromIdProxy(accountIdProxy);

            User::UserPtr _uPtr = UserManager::Instance()->findUserById(accountId);

            if (rsp.retcode() == pb::TryEnterRoomScRsp::RET_SUCC)
            {
                assert(_uPtr.get() != nullptr);

                _uPtr->setRoomId(rsp.roomid());
                _uPtr->setServerNetId(_netId);

                if (!_uPtr->isConnected())
                {
                    // 用户断线时主动推送一遍断线消息
                    NodeService::NotifyUserDisConnect(accountIdProxy, rsp.roomid());
                }
            }

            if (_uPtr->isConnected())
            {
                MsgPtr _rspptr(static_cast<Message *>(new pb::SSGame2GEnterRoomRsp()));
                ((pb::SSGame2GEnterRoomRsp *)(_rspptr.get()))->set_request_id(_uPtr->GetGateNetId());
                pb::TryEnterRoomScRsp *scrsp = new pb::TryEnterRoomScRsp(rspProxy.rsp());
                ((pb::SSGame2GEnterRoomRsp *)(_rspptr.get()))->set_allocated_rsp(scrsp);
                GateService::ResponseWithEnterRoom(_uPtr->GetClientNetId(), _rspptr);
            }
            _uPtr->setCurrentSt(UserState::IDLE);
        }
    }

    void NodeService::onNodeLeaveRoomRsp(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void NodeService::onNodeEnterRoomRsp {}", _netId);

        pb::SSN2GLeaveRoomRsp rspProxy;

        if (rspProxy.ParseFromString(*_ptr.get()))
        {
            pb::TryLeaveRoomScRsp rsp = rspProxy.rsp();

            uint64_t accountIdProxy = rspProxy.accountid();

            UserId accountId = GameServer::Instance()->ParseIdFromIdProxy(accountIdProxy);

            User::UserPtr _uPtr = UserManager::Instance()->findUserById(accountId);

            // 用户必须是存在的，否则认为出现bug，或者是其它系统的虚假消息
            // assert(_uPtr.get() != nullptr);
            if (!_uPtr)
            {
                return;
            }

            if (rsp.retcode() == pb::TryLeaveRoomScRsp::RET_ROOM_BUSY)
            {
                if (!_uPtr->isConnected())
                {
                    NodeService::NotifyUserDisConnect(accountId, _uPtr->GetRoomId());
                }
            }
            else if (rsp.retcode() == pb::TryLeaveRoomScRsp::RET_SUCC)
            {
                // 其它情况对于服务器端无变化，无需推送断线消息
                // if (_uPtr->GetRoomId() == rspProxy.roomid())
                // {
                //     _u
                // }
                _uPtr->setRoomId(-1);
            }

            if (_uPtr->isConnected())
            {
                MsgPtr _rspptr(static_cast<Message *>(new pb::SSGame2GLeaveRoomRsp()));
                ((pb::SSGame2GLeaveRoomRsp *)(_rspptr.get()))->set_request_id(_uPtr->GetGateNetId());
                pb::TryLeaveRoomScRsp *scrsp = new pb::TryLeaveRoomScRsp(rspProxy.rsp());
                ((pb::SSGame2GLeaveRoomRsp *)(_rspptr.get()))->set_allocated_rsp(scrsp);
                GateService::ResponseWithLeaveRoom(_uPtr->GetClientNetId(), _rspptr);
            }
            _uPtr->setCurrentSt(UserState::IDLE);
        }
    }

    void NodeService::onNodeChooseLevelRsp(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void NodeService::onNodeChooseLevelRsp {}", _netId);

        pb::SSN2GChooseLevelRsp rspProxy;

        if (rspProxy.ParseFromString(*_ptr.get()))
        {
            pb::ChooseLevelScRsp rsp = rspProxy.rsp();

            uint64_t accountIdProxy = rspProxy.accountid();

            UserId accountId = GameServer::Instance()->ParseIdFromIdProxy(accountIdProxy);

            User::UserPtr _uPtr = UserManager::Instance()->findUserById(accountId);

            // 用户必须是存在的，否则认为出现bug，或者是其它系统的虚假消息
            // assert(_uPtr.get() != nullptr);
            if (!_uPtr)
            {
                return;
            }

            if (!_uPtr->isConnected())
            {
                NodeService::NotifyUserDisConnect(accountId, _uPtr->GetRoomId());
            }
            _uPtr->setServerNetId(_netId);

            if (_uPtr->isConnected())
            {
                MsgPtr _rspptr(static_cast<Message *>(new pb::SSGame2GChooseLevelRsp()));
                ((pb::SSGame2GChooseLevelRsp *)(_rspptr.get()))->set_request_id(_uPtr->GetGateNetId());
                pb::ChooseLevelScRsp *scrsp = new pb::ChooseLevelScRsp(rspProxy.rsp());
                ((pb::SSGame2GChooseLevelRsp *)(_rspptr.get()))->set_allocated_rsp(scrsp);
                GateService::ResponseWithChooseLevel(_uPtr->GetClientNetId(), _rspptr);
            }
            _uPtr->setCurrentSt(UserState::IDLE);
        }
    }
    void NodeService::onNodeReadyInRoomRsp(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void NodeService::onNodeReadyInRoomRsp {}", _netId);

        pb::SSN2GReadyInRoomRsp rspProxy;

        if (rspProxy.ParseFromString(*_ptr.get()))
        {
            pb::ReadyInRoomScRsp rsp = rspProxy.rsp();

            uint64_t accountIdProxy = rspProxy.accountid();

            UserId accountId = GameServer::Instance()->ParseIdFromIdProxy(accountIdProxy);

            User::UserPtr _uPtr = UserManager::Instance()->findUserById(accountId);

            // 用户必须是存在的，否则认为出现bug，或者是其它系统的虚假消息,消息也应该进行一层校验，防止是非法发送的消息，暂不做这个校验
            // assert(_uPtr.get() != nullptr);
            if (!_uPtr)
            {
                return;
            }

            if (!_uPtr->isConnected())
            {
                NodeService::NotifyUserDisConnect(accountId, _uPtr->GetRoomId());
            }

            _uPtr->setServerNetId(_netId);

            if (_uPtr->isConnected())
            {
                MsgPtr _rspptr(static_cast<Message *>(new pb::SSGame2GReadyRoomRsp()));
                ((pb::SSGame2GReadyRoomRsp *)(_rspptr.get()))->set_request_id(_uPtr->GetGateNetId());
                pb::ReadyInRoomScRsp *scrsp = new pb::ReadyInRoomScRsp(rspProxy.rsp());
                ((pb::SSGame2GReadyRoomRsp *)(_rspptr.get()))->set_allocated_rsp(scrsp);
                GateService::ResponseWithReadyRoom(_uPtr->GetClientNetId(), _rspptr);
            }
            _uPtr->setCurrentSt(UserState::IDLE);
        }
    }

    void NodeService::onUserDisConnectTimeOutNotify(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void NodeService::onUserDisConnectTimeOutNotify {}", _netId);

        pb::SSG2NUserDisConnectRoomNotify notify;
        if (notify.ParseFromString(*_ptr.get()))
        {
            uint64_t accountid = notify.accountid();
            RoomId roomid = notify.roomid();

            // 用户离线
            UserId uid = GameServer::Instance()->ParseIdFromIdProxy(accountid);

            User::UserPtr _uptr = UserManager::Instance()->findUserById(uid);

            if (_uptr && _uptr->GetRoomId() == roomid)
            {
                // 用户彻底离线，假设在这个过程中用户登陆上来了怎么办？
                // UserManager::Instance()->RemoveUserById(uid);

                _uptr->setRoomId(-1);
                _uptr->setCurrentSt(UserState::IDLE);

                if (!_uptr->isConnected())
                {
                    UserManager::Instance()->RemoveUserById(uid);
                }
            }
        }
    }

    void NodeService::NotifyUserDisConnect(const UserId &_userId, const RoomId &_roomId)
    {
        User::UserPtr _uptr = UserManager::Instance()->findUserById(_userId);

        if (_uptr && _uptr->GetRoomId() == _roomId && _uptr->GetServerNetId() != 0)
        {
            // 通知用户离线
            MsgPtr _ptr(static_cast<Message *>(new pb::SSG2NUserDisConnectRoomNotify()));

            uint64_t idProxy = GameServer::Instance()->GetAccountId(_userId);
            ((pb::SSG2NUserDisConnectRoomNotify *)(_ptr.get()))->set_accountid(idProxy);
            ((pb::SSG2NUserDisConnectRoomNotify *)(_ptr.get()))->set_roomid(_roomId);

            GameServer::Instance()->send_message_by_id(_uptr->GetServerNetId(), pb::SS_GAME_NODE_USER_DISCONNECT_NOTIFY, _ptr);
        }
    }

}