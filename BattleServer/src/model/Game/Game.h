#ifndef MINIPROJECT_GAME_HPP
#define MINIPROJECT_GAME_HPP

#include "../typedef.h"
#include <vector>
#include "Player.h"
#include "Task/Mission.h"
#include <sys/time.h>
#include "Event/GameEvent.h"
#include <atomic>

namespace MiniProject
{
    // const uint32_t MISSION_REFRESH_TIME = 1;
    // const uint32_t SCORE_INCRESE_TIME = 5; // 每秒增加的分数
    // const uint32_t SCORE_PLAYER_DEAD = 20; // 玩家死亡扣分
    // const uint32_t SCORE_PUBLISH_TIME = 5; // 分数推送时间

    enum MissionState
    {
        MissionUnRefresh = 0, // 尚未产生任务
        MissionRunning = 1,   // 任务进行中
        MissionFlushInCD = 2  // 任务结束后等待
    };

    enum Event
    {
        EVENT_KILL = 1,
        EVENT_CROSSBLUEFIRE = 2,
        EVENT_BEARSUSTAIN = 3,
        EVENT_HITBOX = 4,
        EVENT_DISCOVER = 5
    };

    enum GameState
    {
        GAME_DEFAULT = 0,
        GAME_PAUSING = 1,
        GAME_RUNNING = 2
    };

    class Room;

    class Game
    {
    public:
        typedef std::shared_ptr<Game> GamePtr;

    public:
        // Game(Room *_ptr, const timeval &tt);
        Game() {}

        Game(RoomId roomid);

        int addPlayer(PlayerId pid, UserId uid);

        int removePlayer(UserId id);

        ~Game();

        void GameStart();
        void GamePause();
        void GameEnd();

        void addPlayer(Player::PlayerPtr _player);
        void randomMissionGenerate();
        void MissionStart();

        bool isMissionTimeOut();

        std::vector<Player::PlayerPtr> GetAllPlayers()
        {
            return playersVec_;
        }

        int GetScore();

        int GetScoreByTime(const timeval &tt);

        int GetScoreId();

        int GetMissionType();
        int GetMissionTypeId();
        int GetMissionTime();

        int finishedTalk(long long playerId);

        int update(const timeval &tt);

        int update(const GameEvent &_event, const timeval &tt);

        void settle(const timeval &tt);

        int step();

        RoomId GetRoomId()
        {
            return roomid_;
        }

        GameId GetGameId()
        {
            return gameid_;
        }

        void setToken(const std::string &_token)
        {
            token = _token;
        }

        bool checkToken(const std::string &_token) const
        {
            return token == _token;
        }

    private:
        int hasFinishedTalkCount = 0;

    private:
        GameState gState;
        long long continue_time = 0;
        long long pause_time = 0;

    private:
        void *gamePlay = nullptr;
        std::string token;

    private:
        RoomId roomid_;
        GameId gameid_;
        bool isFirstMission = true;
        std::vector<Player::PlayerPtr> playersVec_;
        Mission::MissionPtr currentMission_;

        timeval lastRefreshTime;
        timeval lastTaskFinishTime;
        timeval lastGetScoreTime;

        PlayerId missionPlayer;
        int missionTarget;
        unsigned long long levelid = 0;
        unsigned long long score = 0;
        std::atomic<int> scoreid{1};
    };

}

#endif