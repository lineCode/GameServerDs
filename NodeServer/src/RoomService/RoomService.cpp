#include "RoomService.h"
#include "spdlog/spdlog.h"
#include "../../../common/protobuf/node_room_msg.pb.h"
#include "../../../common/protobuf/game_node_msg.pb.h"
#include "../NodeServer.h"

namespace MiniProject
{

    void RoomService::onCreateRoomRsp(const NetId &_netid, MessagePtr _ptr)
    {
        spdlog::info("void RoomService::onCreateRoomRsp {}", _netid);

        pb::SSR2NCreateRoomRsp rsp;

        if (rsp.ParseFromString(*_ptr.get()))
        {
            uint64_t request_id = rsp.request_id();

            MsgPtr _rspptr(static_cast<Message *>(new pb::SSN2GCreateRoomRsp(rsp.rsp())));

            NodeServer::Instance()->send_message_by_id(request_id, pb::SS_NODE_GAME_CREATEROOM_RSP, _rspptr);
        }
    }

    void RoomService::onEnterRoomRsp(const NetId &_netid, MessagePtr _ptr)
    {
        spdlog::info("void RoomService::onEnterRoomRsp {}", _netid);

        pb::SSR2NCreateRoomRsp rsp;

        if (rsp.ParseFromString(*_ptr.get()))
        {
            uint64_t request_id = rsp.request_id();

            MsgPtr _rspptr(static_cast<Message *>(new pb::SSN2GCreateRoomRsp(rsp.rsp())));

            NodeServer::Instance()->send_message_by_id(request_id, pb::SS_NODE_GAME_ENTERROOM_RSP, _rspptr);
        }
    }

    void RoomService::onLeaveRoomRsp(const NetId &_netid, MessagePtr _ptr)
    {
        spdlog::info("void RoomService::onLeaveRoomRsp {}", _netid);

        pb::SSR2NLeaveRoomRsp rsp;

        if (rsp.ParseFromString(*_ptr.get()))
        {
            uint64_t request_id = rsp.request_id();

            MsgPtr _rspptr(static_cast<Message *>(new pb::SSN2GLeaveRoomRsp(rsp.rsp())));

            NodeServer::Instance()->send_message_by_id(request_id, pb::SS_NODE_GAME_ENTERROOM_RSP, _rspptr);
        }
    }

    void RoomService::onChooseLevelRsp(const NetId &_netid, MessagePtr _ptr)
    {
        spdlog::info("void RoomService::onChooseLevelRsp {}", _netid);

        pb::SSR2NChooseLevelRsp rsp;

        if (rsp.ParseFromString(*_ptr.get()))
        {
            uint64_t request_id = rsp.request_id();

            MsgPtr _rspptr(static_cast<Message *>(new pb::SSN2GChooseLevelRsp(rsp.rsp())));
            NodeServer::Instance()->send_message_by_id(request_id, pb::SS_NODE_GAME_LEAVEROOM_RSP, _rspptr);
        }
    }

    void RoomService::onUserReadyRsp(const NetId &_netid, MessagePtr _ptr)
    {
        spdlog::info("void RoomService::onUserReadyRsp {}", _netid);

        pb::SSR2NReadyInRoomRsp rsp;

        if (rsp.ParseFromString(*_ptr.get()))
        {
            uint64_t request_id = rsp.request_id();

            MsgPtr _rspptr(static_cast<Message *>(new pb::SSN2GReadyInRoomRsp(rsp.rsp())));
            NodeServer::Instance()->send_message_by_id(request_id, pb::SS_NODE_GAME_READYROOM_RSP, _rspptr);
        }
    }

    void RoomService::onUserDisConnectNotify(const NetId &_netid, MessagePtr _ptr)
    {
        spdlog::info("void RoomService::onUserDisConnectNotify {}", _netid);

        pb::SSG2NUserDisConnectRoomNotify _notify;
        if (_notify.ParseFromString(*_ptr.get()))
        {
            uint64_t idProxy = _notify.accountid();
            uint64_t roomid = _notify.roomid();

            

        }
    }
}