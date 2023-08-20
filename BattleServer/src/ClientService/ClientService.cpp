#include "ClientService.h"
#include "../../../common/protobuf/client_battle.pb.h"
#include "spdlog/spdlog.h"
#include "../BattleServer.h"
#include "../model/PlayerManager.h"

namespace MiniProject
{

    void ClientService::onClientInStruSync(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void ClientService::onClientInStruSync {}", _netId);

        pb::ClientInputPush clientInput;

        if (clientInput.ParseFromString(*_ptr.get()))
        {
            GameId gid = PlayerManager::Instance()->findGameByNet(_netId);

            Game::GamePtr _gptr = GameManager::Instance()->find(gid);

            if (!_gptr)
            {
                return;
            }

            // 进行游戏同步

            forwardSalveInstruction(_gptr, _ptr);
        }
    }

    void ClientService::onClientStateSync(const NetId &_netId, MessagePtr _ptr)
    {
        spdlog::info("void ClientService::onClientInStruSync {}", _netId);

        pb::WorldSnapshotPush req;

        if (req.ParseFromString(*_ptr.get()))
        {
            GameId gid = PlayerManager::Instance()->findGameByNet(_netId);

            Game::GamePtr _gptr = GameManager::Instance()->find(gid);

            if (!_gptr)
            {
                return;
            }

            // 存储当前游戏状态
            // _gptr->GetWorldSnapShot()+


            forwardHostState(_gptr, _ptr);
        }
    }

    void ClientService::NotifyGameStateChange(const NetId &_netId, MsgPtr _ptr)
    {
    }

    void ClientService::SyncGameStatus(const NetId &_netId, MsgPtr _ptr)
    {
    }

    void ClientService::NotifyUserConnectStateChange(const NetId &_netId, MsgPtr _ptr) // connect/disconnect/join/leave
    {
    }

    void ClientService::onClientLinkGame(const NetId &_netId, MessagePtr _ptr)
    {
        pb::CliTryLinkBattleCsReq req;

        if (req.ParseFromString(*_ptr.get()))
        {
            GameId game = req.gameid();
            UserId user = req.account();
            std::string token = req.token();

            Game::GamePtr _gptr = GameManager::Instance()->find(game);

            // 是否需要检查token
            if (!_gptr || !_gptr->checkToken(token))
            {
                // token不对，禁止加入游戏
                MsgPtr _rspptr(static_cast<Message *>(new pb::CliTryLinkBattleScRsp()));
                ((pb::CliTryLinkBattleScRsp *)(_rspptr.get()))->set_retcode(pb::CliTryLinkBattleScRsp::RET_FAILED);
                ((pb::CliTryLinkBattleScRsp *)(_rspptr.get()))->set_gameid(game);
                ResponseWithLinkBattle(_netId, _rspptr);
            }

            std::vector<Player::PlayerPtr> allPlayers = _gptr->GetAllPlayers();

            Player::PlayerPtr pp;
            for (auto player : allPlayers)
            {
                if (player->GetUserId() == user)
                {
                    pp = player;
                    break;
                }
            }

            if (!pp)
            {
                // token不对，禁止加入游戏
                MsgPtr _rspptr(static_cast<Message *>(new pb::CliTryLinkBattleScRsp()));
                ((pb::CliTryLinkBattleScRsp *)(_rspptr.get()))->set_retcode(pb::CliTryLinkBattleScRsp::RET_FAILED);
                ((pb::CliTryLinkBattleScRsp *)(_rspptr.get()))->set_gameid(game);
                ResponseWithLinkBattle(_netId, _rspptr);
            }

            pp->setNetId(_netId);

            PlayerManager::Instance()->addGameByNet(_netId, game);
            PlayerManager::Instance()->addGameByUser(user, game);

            MsgPtr _rspptr(static_cast<Message *>(new pb::CliTryLinkBattleScRsp()));
            ((pb::CliTryLinkBattleScRsp *)(_rspptr.get()))->set_retcode(pb::CliTryLinkBattleScRsp::RET_SUCC);
            ((pb::CliTryLinkBattleScRsp *)(_rspptr.get()))->set_gameid(game);
            ResponseWithLinkBattle(_netId, _rspptr);

            // 查找游戏状态，进行同步（待添加）

            return;
        }
    }

    void ClientService::ResponseWithLinkBattle(const NetId &_netId, MsgPtr _ptr)
    {
        BattleServer::Instance()->send_message_by_id(_netId, pb::CMDTryLinkBattleScRsp, _ptr);
    }

    void ClientService::forwardSalveInstruction(Game::GamePtr _gptr, MessagePtr _ptr)
    {
        std::vector<Player::PlayerPtr> _players = _gptr->GetAllPlayers();

        for (auto player : _players)
        {
            if (player->PlayId() == 1)
            {
                BattleServer::Instance()->send_message_by_id(player->GetNetId(), pb::CMDCliInstruSync, _ptr);
                break;
            }
        }
    }

    void ClientService::forwardHostState(Game::GamePtr _gptr, MessagePtr _ptr)
    {
        std::vector<Player::PlayerPtr> _players = _gptr->GetAllPlayers();

        for (auto player : _players)
        {
            if (player->PlayId() != 1)
            {
                forwardHostState(player->GetNetId(), _ptr);
            }
        }
    }

    void ClientService::forwardHostState(const NetId &netId, MessagePtr _ptr)
    {
        BattleServer::Instance()->send_message_by_id(netId, pb::CMDCliStatusSyncNotify, _ptr);
    }
}