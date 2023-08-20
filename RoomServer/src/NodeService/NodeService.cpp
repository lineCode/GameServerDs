#include "NodeService.h"
#include <spdlog/spdlog.h>
#include "../../../common/protobuf/node_room_msg.pb.h"
#include "../model/User/User.h"
#include "../RoomServer.h"

// #include

namespace MiniProject
{
    void NodeService::onCreateRoom(const NetId &_netid, MessagePtr _ptr)
    {
        spdlog::info("void NodeService::onCreateRoom {}", _netid);

        pb::SSN2RCreateRoomReq req;

        if (req.ParseFromString(*_ptr.get()) && req.has_req())
        {
            uint64_t req_id = req.request_id();

            pb::SSG2NCreateRoomReq crreq = req.req();
            pb::TryCreateRoomCsReq tryCreateRoomReq = crreq.req();

            std::string room_name = tryCreateRoomReq.name();
            uint32_t usercount = tryCreateRoomReq.maxusercount();

            UserId _netid = crreq.accountid();

            // 定义回复报文
            MsgPtr _rspptr(static_cast<Message *>(new pb::SSR2NCreateRoomRsp));

            pb::SSN2GCreateRoomRsp *ngrsp = new pb::SSN2GCreateRoomRsp();
            ngrsp->set_accountid(_netid);

            pb::TryCreateRoomScRsp *scrsp = new pb::TryCreateRoomScRsp();

            ngrsp->set_allocated_rsp(scrsp);
            ((pb::SSR2NCreateRoomRsp *)(_rspptr.get()))->set_allocated_rsp(ngrsp);

            // 如果用户存在，则需要判断下其是否在房间，实际上这一部处理应该在gameserver中完成
            // if (_uptr)
            // {
            //     if (_uptr->isInRoom())
            //     {
            //         scrsp->set_retcode(pb::TryCreateRoomScRsp::RET_USER_IN_ROOM);
            //         ResponseWithCreateRoom(_netid, _rspptr);
            //         return;
            //     }
            // }

            RoomId _roomid = RoomManager::Instance()->GenerateRoomID(RoomServer::Instance()->GetUid());

            Room::RoomPtr _roomptr = std::make_shared<Room>(_roomid, room_name, usercount);

            if (RoomManager::Instance()->findRoomForCreate(_roomid, _roomptr))
            {
                User::UserPtr _uptr = std::make_shared<User>(_netid);
                _uptr->set_conv_id(_netid);
                _roomptr->userEnter(_uptr);
                scrsp->set_retcode(pb::TryCreateRoomScRsp::RET_SUCC);
                scrsp->set_roomid(_roomptr->GetRoomId());
                scrsp->set_maxusercount(_roomptr->MAX_USER_COUNT());
                scrsp->set_level(_roomptr->Level());
                ResponseWithCreateRoom(_netid, _rspptr);
                return;
            }

            // 按理说下面的逻辑不存在
            spdlog::error("void NodeService::onCreateRoom: {} created error", _roomid);
            scrsp->set_retcode(pb::TryCreateRoomScRsp::RET_ROOMID_EXIST);
            ResponseWithCreateRoom(_netid, _rspptr);
            return;
        }
    }

    void NodeService::ResponseWithCreateRoom(const NetId &_netid, MsgPtr _ptr)
    {
        spdlog::info("void NodeService::ResponseWithCreateRoom {}", _netid);

        RoomServer::Instance()->send_message_by_id(_netid, pb::SS_ROOM_NODE_CREATEROOM_RSP, _ptr);
    }

    void NodeService::onEnterRoom(const NetId &_netid, MessagePtr _ptr)
    {
        spdlog::info("void NodeService::onCreateRoom {}", _netid);

        pb::SSN2REnterRoomReq req;

        if (req.ParseFromString(*_ptr.get()))
        {
            uint64_t request_id = req.request_id();

            pb::SSG2NEnterRoomReq g2nreq = req.req();
            UserId _accountid = g2nreq.accountid();

            pb::TryEnterRoomCsReq csReq = g2nreq.req();

            // 处理下回复的报文
            MsgPtr _rspptr(static_cast<Message *>(new pb::SSR2NEnterRoomRsp()));
            ((pb::SSR2NEnterRoomRsp *)(_rspptr.get()))->set_request_id(request_id);

            pb::SSN2GEnterRoomRsp *n2grsp = new pb::SSN2GEnterRoomRsp();
            n2grsp->set_accountid(_accountid);

            pb::TryEnterRoomScRsp *g2grsp = new pb::TryEnterRoomScRsp();

            n2grsp->set_allocated_rsp(g2grsp);
            ((pb::SSR2NEnterRoomRsp *)(_rspptr.get()))->set_allocated_rsp(n2grsp);

            RoomId _roomid = csReq.roomid();

            Room::RoomPtr _roomPtr = RoomManager::Instance()->findRoomByRoomId(_roomid);

            // 房间不存在
            if (!_roomPtr)
            {
                g2grsp->set_retcode(pb::TryEnterRoomScRsp::RET_ROOM_NOT_EXIST);
                ResponseWithEnterRoom(_netid, _rspptr);
                return;
            }

            User::UserPtr _uPtr = std::make_shared<User>(_accountid);
            _uPtr->set_conv_id(_netid);
            int _result = _roomPtr->userEnter(_uPtr);

            // 用户加入房间失败
            if (_result != 0)
            {
                switch (_result)
                {
                case 1:
                {
                    g2grsp->set_retcode(pb::TryEnterRoomScRsp::RET_ROOM_FULL);
                    break;
                }
                case 2:
                {
                    // 理论上无法进入该段代码
                    g2grsp->set_retcode(pb::TryEnterRoomScRsp::RET_IN_ROOM);
                    g2grsp->set_roomid(_roomid);
                    _uPtr->set_room_id(_roomid);
                    break;
                }
                case 3:
                {
                    g2grsp->set_retcode(pb::TryEnterRoomScRsp::RET_IN_ROOM);
                    break;
                }
                case 4:
                {
                    g2grsp->set_retcode(pb::TryEnterRoomScRsp::RET_ROOM_BUSY);
                    break;
                }
                }
                ResponseWithEnterRoom(_netid, _rspptr);
                return;
            }
            std::cout << "成功加入房间" << std::endl;

            g2grsp->set_retcode(pb::TryEnterRoomScRsp::RET_SUCC);
            g2grsp->set_roomid(_roomPtr->GetRoomId());
            g2grsp->set_maxusercount(_roomPtr->MAX_USER_COUNT());
            g2grsp->set_level(_roomPtr->Level());
            ResponseWithEnterRoom(_netid, _rspptr);
        }
    }

    void NodeService::ResponseWithEnterRoom(const NetId &_netid, MsgPtr _ptr)
    {
        spdlog::info("void NodeService::ResponseWithEnterRoom {}", _netid);

        RoomServer::Instance()->send_message_by_id(_netid, pb::SS_ROOM_NODE_ENTERROOM_RSP, _ptr);
    }

    void NodeService::onLeaveRoom(const NetId &_netid, MessagePtr _ptr)
    {
        spdlog::info("void NodeService::onLeaveRoom {}", _netid);

        pb::SSN2RLeaveRoomReq req;

        if (req.ParseFromString(*_ptr.get()))
        {
            uint64_t request_id = req.request_id();

            pb::SSG2NLeaveRoomReq g2nReq = req.req();
            UserId accountid = g2nReq.accountid();

            pb::TryLeaveRoomCsReq cliReq = g2nReq.req();

            MsgPtr _rspptr(static_cast<Message *>(new pb::SSR2NLeaveRoomRsp()));
            ((pb::SSR2NLeaveRoomRsp *)(_rspptr.get()))->set_request_id(request_id);

            pb::SSN2GLeaveRoomRsp *n2gRsp = new pb::SSN2GLeaveRoomRsp();
            n2gRsp->set_accountid(accountid);

            pb::TryLeaveRoomScRsp *cliRsp = new pb::TryLeaveRoomScRsp();
            n2gRsp->set_allocated_rsp(cliRsp);
            ((pb::SSR2NLeaveRoomRsp *)(_rspptr.get()))->set_allocated_rsp(n2gRsp);

            RoomId _roomid = cliReq.roomid();

            Room::RoomPtr _roomPtr = RoomManager::Instance()->findRoomByRoomId(_roomid);

            if (!_roomPtr)
            {
                cliRsp->set_retcode(pb::TryLeaveRoomScRsp::RET_REQ_INVALID);
                ResponseWithLeaveRoom(_netid, _rspptr);
                return;
            }

            int retcode = _roomPtr->userLeave(accountid);

            if (retcode != 0)
            {
                switch (retcode)
                {
                case 1:
                {
                    cliRsp->set_retcode(pb::TryLeaveRoomScRsp::RET_ROOM_BUSY);
                    break;
                }
                case 2:
                {
                    cliRsp->set_retcode(pb::TryLeaveRoomScRsp::RET_NOT_IN_ROOM);
                    break;
                }
                }
                ResponseWithLeaveRoom(_netid, _rspptr);
                return;
            }

            cliRsp->set_retcode(pb::TryLeaveRoomScRsp::RET_SUCC);
            ResponseWithLeaveRoom(_netid, _rspptr);
            NotifyRoomTotalUsers(_roomPtr);
        }
    }

    void NodeService::ResponseWithLeaveRoom(const NetId &_netid, MsgPtr _ptr)
    {
        spdlog::info("void NodeService::ResponseWithLeaveRoom {}", _netid);

        RoomServer::Instance()->send_message_by_id(_netid, pb::SS_ROOM_NODE_LEAVEROOM_RSP, _ptr);
    }

    void NodeService::onUserReady(const NetId &_netid, MessagePtr _ptr)
    {
        spdlog::info("void NodeService::onUserReady {}", _netid);

        pb::SSN2RReadyInRoomReq req;

        if (req.ParseFromString(*_ptr.get()))
        {
            uint64_t request_id = req.request_id();

            pb::SSG2NReadyInRoomReq g2nReq = req.req();
            UserId accountid = g2nReq.accountid();

            pb::ReadyInRoomCsReq cliReq = g2nReq.req();

            MsgPtr _rspptr(static_cast<Message *>(new pb::SSR2NReadyInRoomRsp()));
            ((pb::SSR2NReadyInRoomRsp *)(_rspptr.get()))->set_request_id(request_id);

            pb::SSN2GReadyInRoomRsp *n2gRsp = new pb::SSN2GReadyInRoomRsp();
            n2gRsp->set_accountid(accountid);

            pb::ReadyInRoomScRsp *cliRsp = new pb::ReadyInRoomScRsp();

            n2gRsp->set_allocated_rsp(cliRsp);
            ((pb::SSR2NReadyInRoomRsp *)(_rspptr.get()))->set_allocated_rsp(n2gRsp);

            // UserId accountid = cliReq.accountid();//accountid使用加长的
            RoomId roomid = cliReq.roomid();

            Room::RoomPtr _roomPtr = RoomManager::Instance()->findRoomByRoomId(roomid);

            if (!_roomPtr)
            {
                cliRsp->set_retcode(pb::ReadyInRoomScRsp::RET_REQ_INVALID);

                ResponseWithUserReady(_netid, _rspptr);

                return;
            }

            int ret = _roomPtr->userReady(accountid);

            std::cout << "Ready Ret:" << ret << std::endl;

            if (ret > 1)
            {
                switch (ret)
                {
                case 2:
                {
                    cliRsp->set_retcode(pb::ReadyInRoomScRsp::RET_FAILED);
                    break;
                }
                case 3:
                {
                    cliRsp->set_retcode(pb::ReadyInRoomScRsp::RET_REQ_INVALID);
                    break;
                }
                case 4:
                {
                    cliRsp->set_retcode(pb::ReadyInRoomScRsp::RET_USER_BEEN_READY);
                    break;
                }
                case 5:
                {
                    cliRsp->set_retcode(pb::ReadyInRoomScRsp::RET_ROLE_NOT_SELECTED);
                    break;
                }
                case 6:
                {
                    cliRsp->set_retcode(pb::ReadyInRoomScRsp::RET_USER_NOT_READY);
                    break;
                }
                }
                ResponseWithUserReady(_netid, _rspptr);
                return;
            }

            cliRsp->set_retcode(pb::ReadyInRoomScRsp::RET_SUCC);

            ResponseWithUserReady(_netid, _rspptr);

            if (ret == 0)
            {
                // NotifyUserReady(_roomPtr, _userId);
                NotifyRoomTotalUsers(_roomPtr);
            }
            else if (ret == 1)
            {
                _roomPtr->set_status(RoomState::RoomStateRunning);
                NotifyGameStart(_roomPtr);
            }
            return;
        }
    }

    void NodeService::ResponseWithUserReady(const NetId &_netid, MsgPtr _ptr)
    {
        spdlog::info("void NodeService::ResponseWithUserReady {}", _netid);

        RoomServer::Instance()->send_message_by_id(_netid, pb::SS_ROOM_NODE_READYROOM_RSP, _ptr);
    }

    void NodeService::onChooseLevel(const NetId &_netid, MessagePtr _ptr)
    {
        spdlog::info("void NodeService::onChooseLevel {}", _netid);

        pb::SSN2RChooseLevelReq req;

        if (req.ParseFromString(*_ptr.get()))
        {
            uint64_t request_id = req.request_id();

            pb::SSG2NChooseLevelReq n2gReq = req.req();
            UserId accountid = n2gReq.accountid();

            pb::ChooseLevelCsReq cliReq = n2gReq.req();

            MsgPtr _rspptr(static_cast<Message *>(new pb::SSR2NChooseLevelRsp()));
            ((pb::SSR2NChooseLevelRsp *)(_rspptr.get()))->set_request_id(request_id);

            pb::SSN2GChooseLevelRsp *n2gRsp = new pb::SSN2GChooseLevelRsp();
            n2gRsp->set_accountid(accountid);

            pb::ChooseLevelScRsp *cliRsp = new pb::ChooseLevelScRsp();
            n2gRsp->set_allocated_rsp(cliRsp);
            ((pb::SSR2NChooseLevelRsp *)(_rspptr.get()))->set_allocated_rsp(n2gRsp);

            RoomId _roomid = cliReq.roomid();
            LevelId _levelid = cliReq.levelid();
            Room::RoomPtr _roomPtr = RoomManager::Instance()->findRoomByRoomId(_roomid);

            if (!_roomPtr)
            {
                ((pb::ChooseLevelScRsp *)(_rspptr.get()))->set_retcode(pb::ChooseLevelScRsp::RET_REQ_INVALID);
                ResponseWithChooseLevel(_netid, _rspptr);
                return;
            }

            // 房间设置关卡
            int ret = _roomPtr->userChoseLevel(accountid, _levelid);

            if (ret != 0)
            {
                switch (ret)
                {
                case 1:
                {
                    cliRsp->set_retcode(pb::ChooseLevelScRsp::RET_ROOM_BUSY);
                    break;
                }
                case 2:
                {
                    cliRsp->set_retcode(pb::ChooseLevelScRsp::RET_REQ_INVALID);
                    break;
                }
                case 3:
                {
                    cliRsp->set_retcode(pb::ChooseLevelScRsp::RET_REQ_INVALID);
                    break;
                }
                case 4:
                {
                    cliRsp->set_retcode(pb::ChooseLevelScRsp::RET_LEVEL_NOT_EXIST);
                    break;
                }
                }
                ResponseWithChooseLevel(_netid, _rspptr);
                return;
            }
            cliRsp->set_retcode(pb::ChooseLevelScRsp::RET_SUCC);
            ResponseWithChooseLevel(_netid, _rspptr);
            NotifyRoomTotalUsers(_roomPtr);
        }
    }

    void NodeService::ResponseWithChooseLevel(const NetId &_netid, MsgPtr _ptr)
    {
        spdlog::info("void NodeService::ResponseWithChooseLevel {}", _netid);

        RoomServer::Instance()->send_message_by_id(_netid, pb::SS_ROOM_NODE_CHOOSELEVEL_RSP, _ptr);
    }

    void NodeService::NotifyRoomTotalUsers(Room::RoomPtr _ptr)
    {
        // 推送过程中通过acccountid查找用户的服务节点,待补充
        // 房间发生变化时直接推送房间内玩家的全部信息，由客户端进行处理
    }

    void NodeService::NotifyGameStart(Room::RoomPtr _ptr)
    {
        // 推送游戏开始的消息
    }
}