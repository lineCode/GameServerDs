#include "GateService.h"

#include "../../../common/protobuf/CsCmd.pb.h"
#include "../../../common/protobuf/cmdID.pb.h"
#include "../../../common/protobuf/game_node_msg.pb.h"
#include "../../../common/protobuf/gate_game_msg.pb.h"
#include "../DBHandle/dbHandle.h"
#include "../GameServer.h"
#include "../NodeService/NodeService.h"
#include "../model/GameConnectionManager.h"
#include "../model/User/UserManager.h"
#include "spdlog/spdlog.h"
#include <iostream>

namespace MiniProject
{

    void GateService::onGateLogin(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("onGateLogin:{}", _netId);

        pb::SSGateLoginReq reqProxy;

        if (reqProxy.ParseFromString(*_ptr.get()))
        {
            pb::LoginCsReq req = reqProxy.req();

            std::string _username = req.name();
            uint32_t _accountid = req.accountid();
            MsgPtr _rspptr(new pb::SSGateLoginRsp);
            ((pb::SSGateLoginRsp *)(_rspptr.get()))->set_request_id(reqProxy.requsted_id());
            pb::LoginScRsp *clientRsp = new pb::LoginScRsp();
            clientRsp->set_accountid(_accountid);
            clientRsp->set_isinroom(false);
            clientRsp->set_isbattling(false);
            ((pb::SSGateLoginRsp *)(_rspptr.get()))->set_allocated_rsp(clientRsp);
            // 验证账号信息
            uint32_t account = DbHandle::GetUserUid(_username);
            if (account == 0 || account != _accountid)
            {
                clientRsp->set_retcode(pb::LoginScRsp::RET_ACCOUNT_ERROR);
                ResponseWithGateLogin(_netId, _rspptr);
                return;
            }

            // 查询账号登录的状态
            User::UserPtr _uptr = UserManager::Instance()->findUserById(_accountid);
            if (_uptr)
            {
                // 判断用户的在线状态
                if (_uptr->isConnected())
                {
                    // 用户正常在线，判断能否顶号
                    if (_uptr->isBattle())
                    {
                        // 用户处于战斗服中，不能顶号
                        clientRsp->set_retcode(pb::LoginScRsp::RET_IN_GAME);
                        ResponseWithGateLogin(_netId, _rspptr);
                        return;
                    }
                    else
                    {
                        // 通知用户下线
                        NetId cliNetId = _uptr->GetClientNetId();
                        if (cliNetId != 0 && (cliNetId != _netId || _uptr->GetGateNetId() != reqProxy.requsted_id()))
                        {
                            GameConnectionManager::Instance()->removeUser(cliNetId, _uptr->GetUserId());
                            MsgPtr _rspptr(new pb::SSForceUserDisConnectReq);
                            ((pb::SSForceUserDisConnectReq *)(_rspptr.get()))->set_retcode(pb::SSForceUserDisConnectReq::RET_USER_LOGIN);
                            ((pb::SSForceUserDisConnectReq *)(_rspptr.get()))->set_accountid(_uptr->GetUserId());
                            ((pb::SSForceUserDisConnectReq *)(_rspptr.get()))->set_request_gc_id(_uptr->GetGateNetId());
                            ((pb::SSForceUserDisConnectReq *)(_rspptr.get()))->set_request_gg_id(cliNetId);
                            // 是一种notify的请求，无需再回应（暂时不处理）
                            GameServer::Instance()->send_message_by_id(cliNetId, pb::SS_GATE_MSG_CMD::SS_GATE_FORCE_USER_DISCONNECT_REQ, _rspptr);
                            _uptr->set_connected(false);
                            GameServer::Instance()->userDisConnected(_uptr);
                        }
                        else if (cliNetId == _netId && _uptr->GetGateNetId() == reqProxy.requsted_id())
                        {
                            clientRsp->set_retcode(pb::LoginScRsp::RET_REQ_INVALID);
                            ResponseWithGateLogin(_netId, _rspptr);
                            return;
                        }

                        _uptr = UserManager::Instance()->findUserById(_accountid);
                        if (_uptr)
                        {
                            // 用户处于创建房间的过程中，暂时只有这种可能（未在战斗且没删除登陆信息）
                            GameConnectionManager::Instance()->addUser(_netId, _uptr->GetUserId());
                            _uptr->set_connected(true);
                            _uptr->setClinetNetId(_netId);
                            _uptr->setGateServerNetId(reqProxy.requsted_id());
                            clientRsp->set_retcode(pb::LoginScRsp::RET_RECONNECT);
                            ResponseWithGateLogin(_netId, _rspptr);
                            return;
                        }
                    }
                }
                else
                {
                    // 用户不在线，但用户数据并未被删除，用户可能处于战斗中，启动重连服务
                    // UserManager::Instance()->updateUserClientId(_uptr, _netId);
                    GameConnectionManager::Instance()->addUser(_netId, _uptr->GetUserId());
                    _uptr->set_connected(true);
                    _uptr->setClinetNetId(_netId);
                    _uptr->setGateServerNetId(reqProxy.requsted_id());
                    clientRsp->set_retcode(pb::LoginScRsp::RET_RECONNECT);
                    clientRsp->set_isinroom(_uptr->isInRoom());
                    clientRsp->set_isbattling(_uptr->isBattle());
                    ResponseWithGateLogin(_netId, _rspptr);
                    onUserReconnectLogin(_netId, _uptr);
                    return;
                }
            }
            // 用户不在线，直接登录即可
            User::UserPtr new_user = std::make_shared<User>(_accountid, _username, _netId);
            new_user->setGateServerNetId(reqProxy.requsted_id());
            if (UserManager::Instance()->addUserById(new_user))
            {
                std::cout << "用户登陆" << std::endl;
                GameConnectionManager::Instance()->addUser(_netId, new_user->GetUserId());
                clientRsp->set_retcode(pb::LoginScRsp::RET_SUCC);
            }
            else
            {
                clientRsp->set_retcode(pb::LoginScRsp::RET_FAILED);
            }
            ResponseWithGateLogin(_netId, _rspptr);
            return;
        }
    }

    void GateService::onUserDisConnect(const NetId &_netid, MessagePtr _ptr)
    {
        spdlog::info("void GateService::onUserDisConnect {}", _netid);

        pb::SSGate2GUserDisConnectNotify notify;

        if (notify.ParseFromString(*_ptr.get()))
        {
            UserId uid = notify.accountid();

            User::UserPtr uptr = UserManager::Instance()->findUserById(uid);

            GameServer::Instance()->userDisConnected(uptr);
        }
    }

    void GateService::onGateUserForceDisConnect(const NetId &_netId, MessagePtr _ptr)
    {
        // 暂不需要，先搁置
        spdlog::info("onGateUserForceDisConnect {}", _netId);

        pb::SSForceUserDisConnectRsp rsp;

        if (rsp.ParseFromString(*_ptr.get()))
        {
            pb::SSForceUserDisConnectRsp::RetCode retcode = rsp.retcode();

            if (retcode == pb::SSForceUserDisConnectRsp::RET_SUCC)
            {
                UserId _uid = rsp.accountid();
                NetId _cliNetId = rsp.request_gc_id();
                NetId _serverId = rsp.request_gg_id();
            }
        }
    }

    void GateService::onUserUpdateNotify(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("onUserUpdateNotify {}", _netId);

        pb::SSAccountInfoNotify userInfo;

        if (userInfo.ParseFromString(*_ptr.get()))
        {
            for (int i = 0; i < userInfo.infos_size(); ++i)
            {
                pb::AccountInfo accountinfo = userInfo.infos(i);
                std::string _username = accountinfo.username();
                UserId _uid = accountinfo.acountid();
                DbHandle::CacheUserInfo(_username, _uid);
            }
        }
    }

    void GateService::ResponseWithGateLogin(const NetId &_netId, MsgPtr _ptr)
    {
        GameServer::Instance()->send_message_by_id(_netId, pb::SS_GATE_MSG_LOGIN_RSP, _ptr);
    }

    void GateService::onUserReconnectLogin(const NetId &_netId, User::UserPtr &_ptr)
    {
        // 用户重连服务

        // 推送用户所处的房间以及相关的数据信息
        if (_ptr->isInRoom())
        {
            // 用户在房间中，需要通知客户端加入房间
            RoomId _rid = _ptr->GetRoomId();

            NetId _serverId = _ptr->GetServerNetId();

            NodeService::pullRoomMsgInfo(_ptr->GetRoomId(), _rid, _serverId);

            // 推送用户所处的战斗服务器以及相关的信息
            if (_ptr->isBattle())
            {
                NodeService::pullBattleMsgInfo(_ptr->GetRoomId(), _rid, _serverId);
            }
        }
    }

    void GateService::ResponseWithRoomMsgNotify(const UserId &_userid, MsgPtr _ptr)
    {
        spdlog::info("ResponseWithPullRoomMsg : {}", _userid);

        User::UserPtr _uptr = UserManager::Instance()->findUserById(_userid);

        if (!_uptr)
        {
            spdlog::error("User {} not exist", _userid);
            return;
        }

        NetId _netid = _uptr->GetClientNetId();

        if (_netid == 0 || !GameServer::Instance()->send_message_by_id(_netid, pb::SS_GAME2GATE_ROOM_INFO_NOTIFY, _ptr))
        {
            spdlog::info("Netid {} not found", _netid);
        }
    }

    void GateService::ResopnseWithBattleMsgNotify(const UserId &_userid, MsgPtr _ptr)
    {
        spdlog::info("ResopnseWithBattleMsgNotify : {}", _userid);

        User::UserPtr _uptr = UserManager::Instance()->findUserById(_userid);

        if (!_uptr)
        {
            spdlog::error("User {} not exist", _userid);
            return;
        }

        NetId _netid = _uptr->GetClientNetId();

        if (_netid == 0 || !GameServer::Instance()->send_message_by_id(_netid, pb::SS_GAME2GATE_BATTLE_INFO_NOTIFY, _ptr))
        {
            spdlog::info("Netid {} not found", _netid);
        }
    }

    void GateService::onGateCreateRoomReq(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void GateService::onGateCreateRoomReq {}", _netId);

        pb::SSGate2GCreateRoomReq reqProxy;

        if (reqProxy.ParseFromString(*_ptr.get()))
        {
            uint32_t request_id = reqProxy.request_id();
            pb::TryCreateRoomCsReq req = reqProxy.req();

            UserId _accountid = req.accountid();
            // roomid暂不使用

            User::UserPtr _uptr = UserManager::Instance()->findUserById(_accountid);

            if (!_uptr || _uptr->GetGateNetId() != request_id || _uptr->is_busy())
            {
                // 不存在该用户，请求非法
                MsgPtr _rspptr(static_cast<Message *>(new pb::SSGame2GCreateRoomRsp));
                ((pb::SSGame2GCreateRoomRsp *)(_rspptr.get()))->set_request_id(request_id);

                pb::TryCreateRoomScRsp *scrsp = new pb::TryCreateRoomScRsp();
                scrsp->set_retcode(pb::TryCreateRoomScRsp::RET_FAILED); // 实际上应该是非法请求
                ((pb::SSGame2GCreateRoomRsp *)(_rspptr.get()))->set_allocated_rsp(scrsp);
                ResponseWithCreateRoom(_netId, _rspptr);
            }
            // 更新下记录的gate的id
            _uptr->setGateServerNetId(request_id);

            std::cout << "doing this" << std::endl;

            uint64_t transUid = GameServer::Instance()->GetAccountId(_accountid);
            MsgPtr _nodeReq(static_cast<Message *>(new pb::SSG2NCreateRoomReq()));

            pb::TryCreateRoomCsReq *tcrreq = new pb::TryCreateRoomCsReq(req);
            ((pb::SSG2NCreateRoomReq *)(_nodeReq.get()))->set_accountid(transUid);
            ((pb::SSG2NCreateRoomReq *)(_nodeReq.get()))->set_allocated_req(tcrreq);

            NetId serverNetID = _uptr->GetServerNetId();

            if (serverNetID == 0)
            {
                std::cout << "fdfdfdoing this" << std::endl;
                GameServer::Instance()->send_message_by_type(ConnectType::CT_NODE_SERVER, pb::SS_GAME_NODE_CREATEROOM_REQ, _nodeReq);
            }
            else
            {
                if (!GameServer::Instance()->send_message_by_id(serverNetID, pb::SS_GAME_NODE_CREATEROOM_REQ, _nodeReq))
                {
                    _uptr->setServerNetId(0);
                    GameServer::Instance()->send_message_by_type(ConnectType::CT_NODE_SERVER, pb::SS_GAME_NODE_CREATEROOM_REQ, _nodeReq);
                }
            }

            _uptr->setCurrentSt(UserState::IN_PROCESS);
        }
    }

    void GateService::onGateEnterRoomReq(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void GateService::onGateEnterRoomReq {}", _netId);

        pb::SSGate2GEnterRoomReq reqProxy;

        if (reqProxy.ParseFromString(*_ptr.get()))
        {
            uint32_t request_id = reqProxy.request_id();
            pb::TryEnterRoomCsReq req = reqProxy.req();

            UserId _accountid = req.accountid();
            RoomId _roomid = req.roomid();

            User::UserPtr _uptr = UserManager::Instance()->findUserById(_accountid);

            if (!_uptr || _uptr->GetClientNetId() != _netId || _uptr->is_busy())
            {
                // 不存在该用户，请求非法
                MsgPtr _rspptr(static_cast<Message *>(new pb::SSGame2GEnterRoomRsp));
                ((pb::SSGame2GEnterRoomRsp *)(_rspptr.get()))->set_request_id(request_id);

                pb::TryEnterRoomScRsp *scrsp = new pb::TryEnterRoomScRsp();
                scrsp->set_retcode(pb::TryEnterRoomScRsp::RET_REQ_INVALID); // 实际上应该是非法请求
                ((pb::SSGame2GEnterRoomRsp *)(_rspptr.get()))->set_allocated_rsp(scrsp);
                ResponseWithEnterRoom(_netId, _rspptr);
                return;
            }
            // 判断下用户在不在房间
            if (_uptr->isInRoom())
            {
                // 用户已经在房间，禁止加入房间
                MsgPtr _rspptr(static_cast<Message *>(new pb::SSGame2GEnterRoomRsp));
                ((pb::SSGame2GEnterRoomRsp *)(_rspptr.get()))->set_request_id(request_id);

                pb::TryEnterRoomScRsp *scrsp = new pb::TryEnterRoomScRsp();
                scrsp->set_retcode(pb::TryEnterRoomScRsp::RET_IN_ROOM); // 实际上应该是非法请求
                ((pb::SSGame2GEnterRoomRsp *)(_rspptr.get()))->set_allocated_rsp(scrsp);
                ResponseWithEnterRoom(_netId, _rspptr);
                return;
            }

            // 更新下记录的gate的id
            _uptr->setGateServerNetId(request_id);

            uint64_t transUid = GameServer::Instance()->GetAccountId(_accountid);
            MsgPtr _nodeReq(static_cast<Message *>(new pb::SSG2NEnterRoomReq()));

            pb::TryEnterRoomCsReq *tcrreq = new pb::TryEnterRoomCsReq(req);
            ((pb::SSG2NEnterRoomReq *)(_nodeReq.get()))->set_accountid(transUid);
            ((pb::SSG2NEnterRoomReq *)(_nodeReq.get()))->set_roomid(_roomid);
            ((pb::SSG2NEnterRoomReq *)(_nodeReq.get()))->set_allocated_req(tcrreq);

            NetId serverNetID = _uptr->GetServerNetId();

            if (serverNetID == 0)
            {
                GameServer::Instance()->send_message_by_type(ConnectType::CT_NODE_SERVER, pb::SS_GAME_NODE_ENTERROOM_REQ, _nodeReq);
            }
            else
            {
                if (!GameServer::Instance()->send_message_by_id(serverNetID, pb::SS_GAME_NODE_ENTERROOM_REQ, _nodeReq))
                {
                    _uptr->setServerNetId(0);
                    GameServer::Instance()->send_message_by_type(ConnectType::CT_NODE_SERVER, pb::SS_GAME_NODE_ENTERROOM_REQ, _nodeReq);
                }
            }

            _uptr->setCurrentSt(UserState::IN_PROCESS);
        }
    }

    void GateService::ResponseWithCreateRoom(const NetId &_netId, MsgPtr _ptr)
    {
        GameServer::Instance()->send_message_by_id(_netId, pb::SS_GAME_GATE_CREATE_ROOM_RSP, _ptr);
    }

    void GateService::ResponseWithEnterRoom(const NetId &_netId, MsgPtr _ptr)
    {
        GameServer::Instance()->send_message_by_id(_netId, pb::SS_GAME_GATE_ENTER_ROOM_RSP, _ptr);
    }

    void GateService::onGateLeaveRoomReq(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void GateService::onGateLeaveRoomReq {}", _netId);

        pb::SSGate2GLeaveRoomReq reqProxy;

        if (reqProxy.ParseFromString(*_ptr.get()))
        {
            uint32_t request_id = reqProxy.request_id();
            pb::TryLeaveRoomCsReq req = reqProxy.req();

            UserId _accountid = req.accountid();
            RoomId _roomid = req.roomid();

            User::UserPtr _uptr = UserManager::Instance()->findUserById(_accountid);

            if (!_uptr || _uptr->GetClientNetId() != _netId)
            {
                // 不存在该用户，请求非法
                MsgPtr _rspptr(static_cast<Message *>(new pb::SSGame2GLeaveRoomRsp));
                ((pb::SSGame2GLeaveRoomRsp *)(_rspptr.get()))->set_request_id(request_id);

                pb::TryLeaveRoomScRsp *scrsp = new pb::TryLeaveRoomScRsp();
                scrsp->set_retcode(pb::TryLeaveRoomScRsp::RET_REQ_INVALID); // 实际上应该是非法请求
                ((pb::SSGame2GLeaveRoomRsp *)(_rspptr.get()))->set_allocated_rsp(scrsp);
                ResponseWithLeaveRoom(_netId, _rspptr);
                return;
            }
            // 判断下用户在不在房间
            if (!_uptr->isInRoom() || (_uptr->GetRoomId() != _roomid) || (_uptr->is_busy()))
            {
                // 用户已经在房间，禁止加入房间
                MsgPtr _rspptr(static_cast<Message *>(new pb::SSGame2GLeaveRoomRsp));
                ((pb::SSGame2GLeaveRoomRsp *)(_rspptr.get()))->set_request_id(request_id);

                pb::TryLeaveRoomScRsp *scrsp = new pb::TryLeaveRoomScRsp();
                if (!_uptr->isInRoom())
                {
                    scrsp->set_retcode(pb::TryLeaveRoomScRsp::RET_NOT_IN_ROOM);
                }
                else
                {
                    scrsp->set_retcode(pb::TryLeaveRoomScRsp::RET_REQ_INVALID);
                }

                ((pb::SSGame2GLeaveRoomRsp *)(_rspptr.get()))->set_allocated_rsp(scrsp);
                ResponseWithLeaveRoom(_netId, _rspptr);
                return;
            }

            // 更新下记录的gate的id
            _uptr->setGateServerNetId(request_id);
            uint64_t transUid = GameServer::Instance()->GetAccountId(_accountid);
            MsgPtr _nodeReq(static_cast<Message *>(new pb::SSG2NLeaveRoomReq()));

            pb::TryLeaveRoomCsReq *tcrreq = new pb::TryLeaveRoomCsReq(req);
            ((pb::SSG2NLeaveRoomReq *)(_nodeReq.get()))->set_accountid(transUid);
            ((pb::SSG2NLeaveRoomReq *)(_nodeReq.get()))->set_roomid(_roomid);
            ((pb::SSG2NLeaveRoomReq *)(_nodeReq.get()))->set_allocated_req(tcrreq);

            NetId serverNetID = _uptr->GetServerNetId();

            if (serverNetID == 0)
            {
                GameServer::Instance()->send_message_by_type(ConnectType::CT_NODE_SERVER, pb::SS_GAME_NODE_LEAVEROOM_REQ, _nodeReq);
            }
            else
            {
                if (!GameServer::Instance()->send_message_by_id(serverNetID, pb::SS_GAME_NODE_LEAVEROOM_REQ, _nodeReq))
                {
                    _uptr->setServerNetId(0);
                    GameServer::Instance()->send_message_by_type(ConnectType::CT_NODE_SERVER, pb::SS_GAME_NODE_LEAVEROOM_REQ, _nodeReq);
                }
            }

            _uptr->setCurrentSt(UserState::IN_PROCESS);
        }
    }

    void GateService::ResponseWithLeaveRoom(const NetId &_netId, MsgPtr _ptr)
    {
        GameServer::Instance()->send_message_by_id(_netId, pb::SS_GAME_GATE_LEAVE_ROOM_RSQ, _ptr);
    }

    void GateService::onGateChooseLevelReq(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void GateService::onGateChooseLevelReq {}", _netId);

        pb::SSGate2GChooseLevelReq reqProxy;

        if (reqProxy.ParseFromString(*_ptr.get()))
        {
            uint32_t request_id = reqProxy.request_id();
            pb::ChooseLevelCsReq req = reqProxy.req();

            UserId _accountid = req.accountid();
            RoomId _roomid = req.roomid();

            User::UserPtr _uptr = UserManager::Instance()->findUserById(_accountid);

            if (!_uptr || _uptr->GetClientNetId() != _netId || _uptr->is_busy())
            {
                // 不存在该用户，请求非法
                MsgPtr _rspptr(static_cast<Message *>(new pb::SSGame2GChooseLevelRsp));
                ((pb::SSGame2GChooseLevelRsp *)(_rspptr.get()))->set_request_id(request_id);

                pb::ChooseLevelScRsp *scrsp = new pb::ChooseLevelScRsp();
                scrsp->set_retcode(pb::ChooseLevelScRsp::RET_REQ_INVALID); // 实际上应该是非法请求
                ((pb::SSGame2GChooseLevelRsp *)(_rspptr.get()))->set_allocated_rsp(scrsp);
                ResponseWithChooseLevel(_netId, _rspptr);
                return;
            }
            // 判断下用户在不在房间
            if (!_uptr->isInRoom() || (_uptr->GetRoomId() != _roomid))
            {
                // 用户已经在房间，禁止加入房间
                MsgPtr _rspptr(static_cast<Message *>(new pb::SSGame2GChooseLevelRsp));
                ((pb::SSGame2GLeaveRoomRsp *)(_rspptr.get()))->set_request_id(request_id);

                pb::ChooseLevelScRsp *scrsp = new pb::ChooseLevelScRsp();
                if (!_uptr->isInRoom())
                {
                    scrsp->set_retcode(pb::ChooseLevelScRsp::RET_REQ_INVALID);
                }
                else
                {
                    scrsp->set_retcode(pb::ChooseLevelScRsp::RET_REQ_INVALID);
                }
                ((pb::SSGame2GChooseLevelRsp *)(_rspptr.get()))->set_allocated_rsp(scrsp);
                ResponseWithChooseLevel(_netId, _rspptr);
                return;
            }

            // 更新下记录的gate的id
            _uptr->setGateServerNetId(request_id);
            uint64_t transUid = GameServer::Instance()->GetAccountId(_accountid);
            MsgPtr _nodeReq(static_cast<Message *>(new pb::SSG2NChooseLevelReq()));

            pb::ChooseLevelCsReq *tcrreq = new pb::ChooseLevelCsReq(req);
            ((pb::SSG2NChooseLevelReq *)(_nodeReq.get()))->set_accountid(transUid);
            ((pb::SSG2NChooseLevelReq *)(_nodeReq.get()))->set_roomid(_roomid);
            ((pb::SSG2NChooseLevelReq *)(_nodeReq.get()))->set_allocated_req(tcrreq);

            NetId serverNetID = _uptr->GetServerNetId();

            if (serverNetID == 0)
            {
                GameServer::Instance()->send_message_by_type(ConnectType::CT_NODE_SERVER, pb::SS_GAME_NODE_CHOOSELEVEL_REQ, _nodeReq);
            }
            else
            {
                if (!GameServer::Instance()->send_message_by_id(serverNetID, pb::SS_GAME_NODE_CHOOSELEVEL_REQ, _nodeReq))
                {
                    _uptr->setServerNetId(0);
                    GameServer::Instance()->send_message_by_type(ConnectType::CT_NODE_SERVER, pb::SS_GAME_NODE_CHOOSELEVEL_REQ, _nodeReq);
                }
            }

            _uptr->setCurrentSt(UserState::IN_PROCESS);
        }
    }

    void GateService::ResponseWithChooseLevel(const NetId &_netId, MsgPtr _ptr)
    {
        GameServer::Instance()->send_message_by_id(_netId, pb::SS_GAME_GATE_CHOOSE_LEVEL_RSP, _ptr);
    }

    void GateService::onGateReadyInRoomReq(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void GateService::onGateReadyInRoomReq {}", _netId);

        pb::SSGate2GReadyRoomReq reqProxy;

        if (reqProxy.ParseFromString(*_ptr.get()))
        {
            uint32_t request_id = reqProxy.request_id();
            pb::ReadyInRoomCsReq req = reqProxy.req();

            UserId _accountid = req.accountid();
            RoomId _roomid = req.roomid();

            User::UserPtr _uptr = UserManager::Instance()->findUserById(_accountid);

            if (!_uptr || _uptr->GetClientNetId() != _netId)
            {
                // 不存在该用户，请求非法
                MsgPtr _rspptr(static_cast<Message *>(new pb::SSGame2GReadyRoomRsp));
                ((pb::SSGame2GReadyRoomRsp *)(_rspptr.get()))->set_request_id(request_id);

                pb::ReadyInRoomScRsp *scrsp = new pb::ReadyInRoomScRsp();
                scrsp->set_retcode(pb::ReadyInRoomScRsp::RET_REQ_INVALID); // 实际上应该是非法请求
                ((pb::SSGame2GReadyRoomRsp *)(_rspptr.get()))->set_allocated_rsp(scrsp);
                ResponseWithReadyRoom(_netId, _rspptr);
                return;
            }
            // 判断下用户在不在房间
            if (!_uptr->isInRoom() || (_uptr->GetRoomId() != _roomid) || _uptr->is_busy())
            {
                // 用户已经在房间，禁止加入房间
                MsgPtr _rspptr(static_cast<Message *>(new pb::SSGame2GReadyRoomRsp));
                ((pb::SSGame2GReadyRoomRsp *)(_rspptr.get()))->set_request_id(request_id);

                pb::ReadyInRoomScRsp *scrsp = new pb::ReadyInRoomScRsp();
                if (!_uptr->isInRoom())
                {
                    scrsp->set_retcode(pb::ReadyInRoomScRsp::RET_REQ_INVALID);
                }
                else
                {
                    scrsp->set_retcode(pb::ReadyInRoomScRsp::RET_REQ_INVALID);
                }
                ((pb::SSGame2GReadyRoomRsp *)(_rspptr.get()))->set_allocated_rsp(scrsp);
                ResponseWithReadyRoom(_netId, _rspptr);
                return;
            }

            // 更新下记录的gate的id
            _uptr->setGateServerNetId(request_id);
            uint64_t transUid = GameServer::Instance()->GetAccountId(_accountid);
            MsgPtr _nodeReq(static_cast<Message *>(new pb::SSG2NReadyInRoomReq()));

            pb::ReadyInRoomCsReq *tcrreq = new pb::ReadyInRoomCsReq(req);
            ((pb::SSG2NReadyInRoomReq *)(_nodeReq.get()))->set_accountid(transUid);
            ((pb::SSG2NReadyInRoomReq *)(_nodeReq.get()))->set_roomid(_roomid);
            ((pb::SSG2NReadyInRoomReq *)(_nodeReq.get()))->set_allocated_req(tcrreq);

            NetId serverNetID = _uptr->GetServerNetId();

            if (serverNetID == 0)
            {
                GameServer::Instance()->send_message_by_type(ConnectType::CT_NODE_SERVER, pb::SS_GAME_NODE_READYROOM_REQ, _nodeReq);
            }
            else
            {
                if (!GameServer::Instance()->send_message_by_id(serverNetID, pb::SS_GAME_NODE_READYROOM_REQ, _nodeReq))
                {
                    _uptr->setServerNetId(0);
                    GameServer::Instance()->send_message_by_type(ConnectType::CT_NODE_SERVER, pb::SS_GAME_NODE_READYROOM_REQ, _nodeReq);
                }
            }

            _uptr->setCurrentSt(UserState::IN_PROCESS);
        }
    }

    void GateService::ResponseWithReadyRoom(const NetId &_netId, MsgPtr _ptr)
    {
        GameServer::Instance()->send_message_by_id(_netId, pb::SS_GAME_GATE_USER_READY_RSP, _ptr);
    }

}