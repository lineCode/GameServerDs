#include "GameService.h"

#include <spdlog/spdlog.h>
#include "../../../common/protobuf/game_node_msg.pb.h"
#include "../../../common/protobuf/node_room_msg.pb.h"

#include "../NodeServer.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace MiniProject
{
    static uint32_t ParseServerIdFromRoomId(uint64_t roomid)
    {
        return (uint32_t)(roomid >> 32);
    }

    void GameService::onGameCreateRoomReq(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void RoomService::onGameCreateRoomReq {}", _netId);

        pb::SSG2NCreateRoomReq *req = new pb::SSG2NCreateRoomReq();

        if (req != nullptr && req->ParseFromString(*_ptr.get()))
        {
            MsgPtr _reqptr(static_cast<Message *>(new pb::SSN2RCreateRoomReq()));
            ((pb::SSN2RCreateRoomReq *)(_reqptr.get()))->set_request_id(_netId);
            ((pb::SSN2RCreateRoomReq *)(_reqptr.get()))->set_allocated_req(req);

            // 根据负载均衡算法查找一个房间服务器将请求发送上去
            if (!NodeServer::Instance()->send_message_by_type(ConnectType::CT_ROOM_SERVER,
                                                              pb::SS_NODE_ROOM_CREATEROOM_REQ, _reqptr))
            {
                // 发送失败，服务不可用
                MsgPtr _rspptr(static_cast<Message *>(new pb::SSN2GCreateRoomRsp()));
                pb::TryCreateRoomScRsp *crRsp = new pb::TryCreateRoomScRsp();
                crRsp->set_retcode(pb::TryCreateRoomScRsp::RET_FAILED);
                ((pb::SSN2GCreateRoomRsp *)(_rspptr.get()))->set_accountid(req->req().accountid());
                ((pb::SSN2GCreateRoomRsp *)(_rspptr.get()))->set_allocated_rsp(crRsp);
                ResponseWithGameCreateRoom(_netId, _rspptr);
            }
            return;
        }
        if (req)
        {
            delete req;
        }
    }

    void GameService::ResponseWithGameCreateRoom(const NetId &_netId, MsgPtr _ptr)
    {
        spdlog::info("void GameService::ResponseWithGameCreateRoom {}", _netId);
        NodeServer::Instance()->send_message_by_id(_netId, pb::SS_NODE_GAME_CREATEROOM_RSP, _ptr);
    }

    void GameService::onGameEnterRoomReq(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void RoomService::onGameEnterRoomReq {}", _netId);

        pb::SSG2NEnterRoomReq *req = new pb::SSG2NEnterRoomReq();

        if (req != nullptr && req->ParseFromString(*_ptr.get()))
        {
            uint64_t _roomid = req->roomid();

            MsgPtr _reqptr(static_cast<Message *>(new pb::SSN2REnterRoomReq()));
            ((pb::SSN2REnterRoomReq *)(_reqptr.get()))->set_request_id(_netId);
            ((pb::SSN2REnterRoomReq *)(_reqptr.get()))->set_allocated_req(req);

            uint32_t suid = ParseServerIdFromRoomId(_roomid);

            if (!NodeServer::Instance()->send_msg_by_uid(suid, pb::SS_NODE_ROOM_ENTERROOM_REQ, _reqptr))
            {
                MsgPtr _rspptr(static_cast<Message *>(new pb::SSN2GEnterRoomRsp()));
                ((pb::SSN2GEnterRoomRsp *)(_rspptr.get()))->set_accountid(req->accountid());

                pb::TryEnterRoomScRsp *rsp = new pb::TryEnterRoomScRsp();
                rsp->set_retcode(pb::TryEnterRoomScRsp::RET_FAILED);

                ResponseWithGameEnterRoom(_netId, _rspptr);
            }
            return;
        }
        if (req)
        {
            delete req;
        }
    }

    void GameService::ResponseWithGameEnterRoom(const NetId &_netId, MsgPtr _ptr)
    {
        spdlog::info("void GameService::ResponseWithGameEnterRoom {}", _netId);
        NodeServer::Instance()->send_message_by_id(_netId, pb::SS_NODE_GAME_ENTERROOM_RSP, _ptr);
    }

    void GameService::onGameLeaveRoomReq(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void RoomService::onGameLeaveRoomReq {}", _netId);

        pb::SSG2NLeaveRoomReq *req = new pb::SSG2NLeaveRoomReq();

        if (req != nullptr && req->ParseFromString(*_ptr.get()))
        {
            MsgPtr _reqptr(static_cast<Message *>(new pb::SSN2RLeaveRoomReq));

            ((pb::SSN2RLeaveRoomReq *)(_reqptr.get()))->set_request_id(_netId);
            ((pb::SSN2RLeaveRoomReq *)(_reqptr.get()))->set_allocated_req(req);

            uint64_t roomid = req->roomid();

            uint32_t suid = ParseServerIdFromRoomId(roomid);

            if (!NodeServer::Instance()->send_msg_by_uid(suid, pb::SS_NODE_ROOM_LEAVEROOM_REQ, _reqptr))
            {
                MsgPtr _rspptr(static_cast<Message *>(new pb::SSN2GLeaveRoomRsp()));

                pb::TryLeaveRoomScRsp *rsp = new pb::TryLeaveRoomScRsp();
                rsp->set_retcode(pb::TryLeaveRoomScRsp::RET_FAILED);

                ((pb::SSN2GLeaveRoomRsp *)(_rspptr.get()))->set_accountid(req->accountid());
                ((pb::SSN2GLeaveRoomRsp *)(_rspptr.get()))->set_allocated_rsp(rsp);

                ResponseWithGameLeaveRoom(_netId, _rspptr);
            }
            return;
        }
        if (req != nullptr)
        {
            delete req;
            req = nullptr;
        }
    }

    void GameService::ResponseWithGameLeaveRoom(const NetId &_netId, MsgPtr _ptr)
    {
        spdlog::info("void GameService::ResponseWithGameLeaveRoom {}", _netId);
        NodeServer::Instance()->send_message_by_id(_netId, pb::SS_NODE_GAME_LEAVEROOM_RSP, _ptr);
    }

    void GameService::onGameChooseLevelReq(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void RoomService::onGameChooseLevelReq {}", _netId);

        pb::SSG2NChooseLevelReq *req = new pb::SSG2NChooseLevelReq();

        if (req != nullptr && req->ParseFromString(*_ptr.get()))
        {
            MsgPtr _roomReq(static_cast<Message *>(new pb::SSN2RChooseLevelReq()));

            ((pb::SSN2RChooseLevelReq *)(_roomReq.get()))->set_request_id(_netId);
            ((pb::SSN2RChooseLevelReq *)(_roomReq.get()))->set_allocated_req(req);

            uint64_t roomid = req->roomid();

            uint32_t suid = ParseServerIdFromRoomId(roomid);

            if (!NodeServer::Instance()->send_msg_by_uid(suid, pb::SS_NODE_ROOM_CHOOSELEVEL_REQ, _roomReq))
            {
                MsgPtr _gameRsp(static_cast<Message *>(new pb::SSN2GChooseLevelRsp()));

                pb::ChooseLevelScRsp *cLRsp = new pb::ChooseLevelScRsp();
                cLRsp->set_retcode(pb::ChooseLevelScRsp::RET_REQ_INVALID);
                ((pb::SSN2GChooseLevelRsp *)(_gameRsp.get()))->set_accountid(req->accountid());
                ((pb::SSN2GChooseLevelRsp *)(_gameRsp.get()))->set_allocated_rsp(cLRsp);

                ResponseWithGameChooseLevel(_netId, _gameRsp);
            }
            return;
        }
        if (req != nullptr)
        {
            delete req;
            req = nullptr;
        }
    }

    void GameService::ResponseWithGameChooseLevel(const NetId &_netId, MsgPtr _ptr)
    {
        spdlog::info("void GameService::ResponseWithGameLeaveRoom {}", _netId);
        NodeServer::Instance()->send_message_by_id(_netId, pb::SS_NODE_GAME_CHOOSELEVEL_RSP, _ptr);
    }

    void GameService::onGameUserReadyReq(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void RoomService::onGameUserReadyReq {}", _netId);

        pb::SSG2NReadyInRoomReq *req = new pb::SSG2NReadyInRoomReq();

        if (req != nullptr && req->ParseFromString(*_ptr.get()))
        {
            MsgPtr _roomReq(static_cast<Message *>(new pb::SSN2RReadyInRoomReq()));

            ((pb::SSN2RReadyInRoomReq *)(_roomReq.get()))->set_request_id(_netId);
            ((pb::SSN2RReadyInRoomReq *)(_roomReq.get()))->set_allocated_req(req);

            uint64_t roomid = req->roomid();

            uint32_t suid = ParseServerIdFromRoomId(roomid);

            if (!NodeServer::Instance()->send_msg_by_uid(suid, pb::SS_NODE_ROOM_READYROOM_REQ, _roomReq))
            {
                MsgPtr _gameRsp(static_cast<Message *>(new pb::SSN2GReadyInRoomRsp()));

                pb::ReadyInRoomScRsp *_rrRsp = new pb::ReadyInRoomScRsp();
                _rrRsp->set_retcode(pb::ReadyInRoomScRsp::RET_FAILED);

                ((pb::SSN2GReadyInRoomRsp *)(_gameRsp.get()))->set_accountid(req->accountid());
                ((pb::SSN2GReadyInRoomRsp *)(_gameRsp.get()))->set_allocated_rsp(_rrRsp);

                ResponseWithGameUserReady(_netId, _gameRsp);
            }
            return;
        }
        if (req != nullptr)
        {
            delete req;
            req = nullptr;
        }
    }

    void GameService::ResponseWithGameUserReady(const NetId &_netId, MsgPtr _ptr)
    {
        spdlog::info("void GameService::ResponseWithGameUserReady {}", _netId);
        NodeServer::Instance()->send_message_by_id(_netId, pb::SS_NODE_GAME_READYROOM_RSP, _ptr);
    }

    void GameService::onPullRoomMsgReq(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void RoomService::onPullBattleMsgReq {}", _netId);
    }

    void GameService::onPullBattleMsgReq(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void RoomService::onPullBattleMsgReq {}", _netId);
    }

    void GameService::onGameUserDisConnect(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void GameService::onGameUserDisConnect {}", _netId);
    }
    void GameService::onGameUserReConnect(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void GameService::onGameUserReConnect {}", _netId);
    }

}