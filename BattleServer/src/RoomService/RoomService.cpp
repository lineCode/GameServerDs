#include "RoomService.h"
#include "../../../common/protobuf/room_battle.pb.h"
#include "spdlog/spdlog.h"
#include "../model/Game/GameManager.h"
#include "../BattleServer.h"
#include "../ClientService/ClientService.h"

namespace MiniProject
{

    void RoomService::onCreateGameRoom(const NetId &_netId, MessagePtr _ptr)
    {
        pb::SSR2BCreateRoomReq req;

        if (req.ParseFromString(*_ptr.get()))
        {
            spdlog::info("Battle Create Game {}", _netId);

            uint64 roomid = req.roomid();
            std::string token = req.token();

            Game::GamePtr gamePtr = GameManager::Instance()->find(roomid);

            if (gamePtr)
            {
                MsgPtr _msgptr(static_cast<Message *>(new pb::SSB2RCreateRoomRsp()));
                ((pb::SSB2RCreateRoomRsp *)(_msgptr.get()))->set_retcode(pb::SSB2RCreateRoomRsp::RET_GAME_EXIST);
                ((pb::SSB2RCreateRoomRsp *)(_msgptr.get()))->set_roomid(roomid);
                ResponseWithCreateGame(_netId, _msgptr);
                return;
            }
            gamePtr = std::make_shared<Game>(roomid);
            gamePtr->setToken(token);

            bool isValid = req.pinfo_size() > 0;
            for (int i = 0; i < req.pinfo_size(); ++i)
            {
                pb::PlayerInfo pinfo = req.pinfo(i);
                if (gamePtr->addPlayer(pinfo.playerid(), pinfo.accountid()) != 0)
                {
                    isValid = false;
                    break;
                }
            }
            if (isValid && !GameManager::Instance()->add(gamePtr))
            {
                isValid = false;
            }
            MsgPtr _msgptr(static_cast<Message *>(new pb::SSB2RCreateRoomRsp()));
            ((pb::SSB2RCreateRoomRsp *)(_msgptr.get()))->set_retcode(isValid ? pb::SSB2RCreateRoomRsp::RET_SUCC : pb::SSB2RCreateRoomRsp::RET_FAILED);
            ((pb::SSB2RCreateRoomRsp *)(_msgptr.get()))->set_roomid(roomid);
            ResponseWithCreateGame(_netId, _msgptr);
            return;
        }
    }

    void RoomService::onJoinGameRoom(const NetId &_netId, MessagePtr _ptr)
    {
        pb::SSR2BJoinRoomReq req;

        if (req.ParseFromString(*_ptr.get()))
        {
            RoomId roomid = req.roomid();
            pb::PlayerInfo pinfo = req.pinfo();

            Game::GamePtr gamePtr = GameManager::Instance()->find(roomid);

            if (!gamePtr)
            {
                MsgPtr _msgptr(static_cast<Message *>(new pb::SSB2RJoinRoomRsp()));
                ((pb::SSB2RJoinRoomRsp *)(_msgptr.get()))->set_retcode(pb::SSB2RJoinRoomRsp::RET_GAME_NOT_EXIST);
                ((pb::SSB2RJoinRoomRsp *)(_msgptr.get()))->set_roomid(roomid);
                ResponseWithJoinGame(_netId, _msgptr);
                return;
            }

            if (gamePtr->addPlayer(pinfo.playerid(), pinfo.accountid()) != 0)
            {
                MsgPtr _msgptr(static_cast<Message *>(new pb::SSB2RJoinRoomRsp()));
                ((pb::SSB2RJoinRoomRsp *)(_msgptr.get()))->set_retcode(pb::SSB2RJoinRoomRsp::RET_FAILED);
                ((pb::SSB2RJoinRoomRsp *)(_msgptr.get()))->set_roomid(roomid);
                ResponseWithJoinGame(_netId, _msgptr);
                return;
            }

            MsgPtr _msgptr(static_cast<Message *>(new pb::SSB2RJoinRoomRsp()));
            ((pb::SSB2RJoinRoomRsp *)(_msgptr.get()))->set_retcode(pb::SSB2RJoinRoomRsp::RET_SUCC);
            ((pb::SSB2RJoinRoomRsp *)(_msgptr.get()))->set_roomid(roomid);
            ResponseWithJoinGame(_netId, _msgptr);
            return;
        }
    }

    void RoomService::onLeaveGameRoom(const NetId &_netId, MessagePtr _ptr)
    {
        pb::SSR2BLeaveRoomReq req;

        if (req.ParseFromString(*_ptr.get()))
        {
            RoomId roomid = req.roomid();
            UserId userid = req.accountid();

            Game::GamePtr gamePtr = GameManager::Instance()->find(roomid);

            if (!gamePtr)
            {
                MsgPtr _msgptr(static_cast<Message *>(new pb::SSB2RLeaveRoomRsp()));
                ((pb::SSB2RLeaveRoomRsp *)(_msgptr.get()))->set_retcode(pb::SSB2RLeaveRoomRsp::RET_GAME_NOT_EXIST);
                ((pb::SSB2RLeaveRoomRsp *)(_msgptr.get()))->set_roomid(roomid);
                ResponseWithLeaveGame(_netId, _msgptr);
                return;
            }

            if (gamePtr->removePlayer(userid) != 0)
            {
                MsgPtr _msgptr(static_cast<Message *>(new pb::SSB2RLeaveRoomRsp()));
                ((pb::SSB2RLeaveRoomRsp *)(_msgptr.get()))->set_retcode(pb::SSB2RLeaveRoomRsp::RET_FAILED);
                ((pb::SSB2RLeaveRoomRsp *)(_msgptr.get()))->set_roomid(roomid);
                ResponseWithLeaveGame(_netId, _msgptr);
                return;
            }

            MsgPtr _msgptr(static_cast<Message *>(new pb::SSB2RLeaveRoomRsp()));
            ((pb::SSB2RLeaveRoomRsp *)(_msgptr.get()))->set_retcode(pb::SSB2RLeaveRoomRsp::RET_SUCC);
            ((pb::SSB2RLeaveRoomRsp *)(_msgptr.get()))->set_roomid(roomid);
            ResponseWithLeaveGame(_netId, _msgptr);

            return;
        }
    }

    void RoomService::ResponseWithCreateGame(const NetId &_netId, MsgPtr _ptr)
    {
        BattleServer::Instance()->send_message_by_id(_netId, pb::SS_BATTLE_CREATE_RSP, _ptr);
    }

    void RoomService::ResponseWithJoinGame(const NetId &_netId, MsgPtr _ptr)
    {
        BattleServer::Instance()->send_message_by_id(_netId, pb::SS_BATTLE_JOIN_RSP, _ptr);
    }

    void RoomService::ResponseWithLeaveGame(const NetId &_netId, MsgPtr _ptr)
    {
        BattleServer::Instance()->send_message_by_id(_netId, pb::SS_BATTLE_LEAVE_RSP, _ptr);
    }

}
