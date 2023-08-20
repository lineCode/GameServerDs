#include "Game.h"
#include <cstdlib>
#include "Title/Title.h"
#include "GameConfig.hpp"
#include <iostream>

namespace MiniProject
{
    // Game::Game(Room *_ptr, const timeval &tt)
    // {
    //     roomid_ = _ptr->GetRoomId();

    //     std::list<User::UserPtr> users = _ptr->GetTotalUsers();

    //     for (User::UserPtr _uptr : users)
    //     {
    //         Player::PlayerPtr ptr = std::make_shared<Player>(_uptr->playerId());
    //         addPlayer(ptr);
    //         std::cout << ptr->PlayId() << std::endl;
    //     }

    //     levelid = _ptr->Level();

    //     lastRefreshTime = tt;
    //     lastTaskFinishTime = tt;
    //     lastGetScoreTime = tt;

    //     continue_time = 90;
    //     if (RoomConfig::Instance()->levelMaps_.count(levelid))
    //     {
    //         continue_time = RoomConfig::Instance()->levelMaps_[levelid];
    //     }

    //     score = GameConfig::SCORE_LEVEL_INIT[levelid - 1];

    //     GameConfig::readfromfile(); // 更新下配置
    // }

    Game::Game(RoomId roomId) : roomid_(roomId), gameid_(roomId)
    {
    }

    /*
        addPlayer:
        return：
                0：添加成功
                -1:游戏正在运行，无法添加玩家
                1：玩家已经存在
    */
    int Game::addPlayer(PlayerId pid, UserId uid)
    {
        if (gState != GameState::GAME_DEFAULT)
        {
            return -1;
        }
        Player::PlayerPtr _ptr = std::make_shared<Player>(pid);
        _ptr->setUserId(uid);

        for (auto player : playersVec_)
        {
            if (player->GetUserId() == uid || player->PlayId() == pid)
            {
                return 1;
            }
        }
        playersVec_.push_back(_ptr);
        return 0;
    }

    /*
        removePlayer:
        return：
                0：删除成功
                -1:游戏正在运行，无法删除玩家
                1：玩家不存在
    */
    int Game::removePlayer(UserId uid)
    {
        if (gState != GameState::GAME_DEFAULT)
        {
            return -1;
        }
        for (auto it = playersVec_.begin(); it != playersVec_.end(); ++it)
        {
            if ((*it)->GetUserId() == uid)
            {
                playersVec_.erase(it);
                return 0;
            }
        }
        return 1;
    }

    void Game::GameStart()
    {
        gState = GameState::GAME_RUNNING;
    }

    void Game::GamePause()
    {
        pause_time = 20;
        gState = GameState::GAME_PAUSING;
    }

    void Game::GameEnd()
    {
        gState = GameState::GAME_DEFAULT;
    }

    int Game::step()
    {
        if (gState == GameState::GAME_RUNNING)
        {
            --continue_time;
            if (continue_time <= 0)
            {
                return 1;
            }
        }
        else if (gState == GameState::GAME_PAUSING)
        {
            --pause_time;
            if (pause_time <= 0)
            {
                return 2;
            }
        }
        return 0;
    }

    Game::~Game() {}

    bool Game::isMissionTimeOut()
    {
        return currentMission_->is_timeout();
    }

    void Game::addPlayer(Player::PlayerPtr _player)
    {
        for (auto &_pp : playersVec_)
        {
            if (_pp == _player)
            {
                return;
            }
        }
        playersVec_.push_back(_player);
    }

    void Game::randomMissionGenerate()
    {
        // 第一个任务必须是团队任务
        if (isFirstMission)
        {
            isFirstMission = false;
            currentMission_.reset(static_cast<Mission *>(new Mission(SURVIVAL_MISSION)));
            return;
        }
        // 配置任务权重，特殊任务和常规任务
        int taskRandom = rand() % 6;
        taskRandom = taskRandom > 2 ? 1 : taskRandom;
        std::cout << "随机任务产生:" << taskRandom << std::endl;

        if (taskRandom == 0)
        {
            currentMission_.reset(static_cast<Mission *>(new Mission(SURVIVAL_MISSION)));
            return;
        }
        else if (taskRandom == 1)
        {
            if (levelid == 1)
            {
                currentMission_.reset(static_cast<Mission *>(new Mission(SPECIAL_MISSION_1)));
                missionPlayer = random() % playersVec_.size() + 1;
            }
            else if (levelid == 2)
            {
                currentMission_.reset(static_cast<Mission *>(new Mission(SPECIAL_MISSION_2)));
                missionPlayer = random() % playersVec_.size() + 1;
                missionTarget = 0;
            }
            else if (levelid == 3)
            {
                currentMission_.reset(static_cast<Mission *>(new Mission(SPECIAL_MISSION_3)));
                missionPlayer = random() % playersVec_.size() + 1;
            }
            return;
        }
        else
        {
            currentMission_.reset(static_cast<Mission *>(new Mission(SINGLE_MISSION)));
            missionPlayer = random() % playersVec_.size() + 1;
        }
    }

    void Game::MissionStart()
    {
        if (currentMission_)
        {
            currentMission_->pubulish();
        }
    }

    int Game::finishedTalk(long long playerId)
    {
        int ret = -1;
        for (auto player : playersVec_)
        {
            if (player->PlayId() == playerId)
            {
                if (!player->hasTalk())
                {
                    player->finishTalk();
                    ++hasFinishedTalkCount;
                    ret = 0;
                    break;
                }
            }
        }
        if (hasFinishedTalkCount == playersVec_.size())
        {
            ret = 1;
        }
        return ret;
    }

    /*
        发生事件时更新状态，返回int
        int:
            0:无事发生
            1：团队生存任务失败
            2：个人生存任务失败
            3：特殊任务完成
            4：特殊任务失败
            5: 分数变动（玩家死亡影响）
    */
    int Game::update(const GameEvent &_event, const timeval &tt)
    {
        int ret = 0;
        uint32_t from = _event.from;
        uint32_t to = _event.to;
        if (from > playersVec_.size())
        {
            return 0;
        }
        if (_event.type_ == GAMEEVENT_KILL)
        {
            if (score >= GameConfig::SCORE_PLAYER_DEAD)
            {
                score -= GameConfig::SCORE_PLAYER_DEAD;
            }
            else
            {
                score = 0;
            }
            ret = -1;
            uint32_t from = _event.from;
            uint32_t to = _event.to;
            if (currentMission_)
            {
                if (currentMission_->Type() == SURVIVAL_MISSION)
                {
                    ret = 1;
                    lastTaskFinishTime = tt;
                    currentMission_.reset();
                }
                else if (currentMission_->Type() == SINGLE_MISSION && from == missionPlayer)
                {
                    ret = 2;
                    lastTaskFinishTime = tt;
                    currentMission_.reset();
                }
            }
            if (to == from || to == 0)
            {
                for (auto &player : playersVec_)
                {
                    if (player->PlayId() == from)
                    {
                        player->suicide();
                    }
                }
            }
            else
            {
                for (auto &player : playersVec_)
                {
                    if (player->PlayId() == to)
                    {
                        player->kill();
                    }
                    else if (player->PlayId() == from)
                    {
                        player->dead();
                    }
                }
            }
        }
        else if (_event.type_ == GAMEVENT_BEARSUSTAIN)
        {
            uint32_t from = _event.from;
            uint64_t stepstart = _event.start;
            uint64_t stepstop = _event.end;

            for (auto &player : playersVec_)
            {
                if (player->PlayId() == from)
                {
                    player->addbeartime(stepstop - stepstart);
                }
            }
        }
        else if (_event.type_ == GAMEEVENT_CROSSBULEFIRE)
        {
            if (currentMission_ && currentMission_->Type() == SPECIAL_MISSION_1)
            {
                uint32_t from = _event.from;
                if (from == missionPlayer)
                {
                    ret = 3;
                    score += currentMission_->GetScore();
                    lastTaskFinishTime = tt;
                    currentMission_.reset();
                }
            }
        }
        else if (_event.type_ == GAMEVENT_HITBOX)
        {
            score += GameConfig::SCORE_PLAYER_HITBOX;
            ret = -1;
            if (currentMission_ && currentMission_->Type() == SPECIAL_MISSION_2)
            {
                uint32_t from = _event.from;
                if (from == missionPlayer)
                {
                    ++missionTarget;
                    if (missionTarget >= currentMission_->GetTarget())
                    {
                        ret = 4;
                        score += currentMission_->GetScore();
                        lastTaskFinishTime = tt;
                        currentMission_.reset();
                    }
                }
            }
        }
        else if (_event.type_ == GAMEVENT_DISCOVER)
        {
            if (currentMission_ && currentMission_->Type() == SPECIAL_MISSION_3)
            {
                uint32_t from = _event.from;
                if (from == missionPlayer)
                {
                    ret = 5;
                    score += currentMission_->GetScore();
                    lastTaskFinishTime = tt;
                    currentMission_.reset();
                }
            }
        }
        return ret;
    }

    /*
        定时更新：刷新任务
        返回值：int
                    0:无事件发生
                    1:产生了新的任务
                    2:生存任务结束（成功）
                    3：特殊任务结束(失败)
    */
    int Game::update(const timeval &tt)
    {
        int secs = std::min(1, (int)(tt.tv_sec - lastRefreshTime.tv_sec));

        lastRefreshTime = tt;

        // score += GameConfig::SCORE_INCRESE_TIME * secs;
        // 把加分迁移到推送里面

        int ret = 0; //

        if (!currentMission_)
        {
            int _secs = tt.tv_sec - lastTaskFinishTime.tv_sec;
            if (_secs >= GameConfig::MISSION_REFRESH_TIME)
            {
                // 需要刷新新的任务
                randomMissionGenerate();
                return 1;
            }
        }
        else if (currentMission_->is_timeout())
        {
            if (currentMission_->Type() == SURVIVAL_MISSION || currentMission_->Type() == SINGLE_MISSION)
            {
                // 任务成功完成
                score += currentMission_->GetScore();
                ret = 2;
            }
            else
            {
                ret = 3;
            }
            lastTaskFinishTime = tt;
            currentMission_.reset();
            return ret;
        }
        return ret;
    }

    void Game::settle(const timeval &tt)
    {
        int ums = std::min(0, (int)(tt.tv_sec - lastGetScoreTime.tv_sec));
        score += ums * GameConfig::SCORE_INCRESE_TIME;
        for (auto &_pp : playersVec_)
        {
            TitleManager manager(this, (Player *)_pp.get());
            Title tt = manager.GetTitle(levelid);
            _pp->set_title(tt);
            score += tt.score;
        }
    }

    int Game::GetScoreId()
    {
        return scoreid;
    }

    int Game::GetScore()
    {
        // lastGetScoreTime = tt;
        scoreid.fetch_add(1);
        return score;
    }

    int Game::GetScoreByTime(const timeval &tt)
    {
        int ums = tt.tv_sec - lastGetScoreTime.tv_sec;

        if (ums >= GameConfig::SCORE_PUBLISH_TIME)
        {
            scoreid.fetch_add(1);
            score += ums * GameConfig::SCORE_INCRESE_TIME;
            lastGetScoreTime = tt;
            return score;
        }
        return -1;
    }

    int Game::GetMissionType()
    {
        return currentMission_->Type();
    }

    int Game::GetMissionTypeId()
    {
        return MissionConfigManager::Instance()->GetMissionId(currentMission_->Type(), missionPlayer);
    }

    int Game::GetMissionTime()
    {
        return currentMission_->Time();
    }
}