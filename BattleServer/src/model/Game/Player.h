#ifndef MINIPROJECT_PLAYER_HPP
#define MINIPROJECT_PLAYER_HPP

#include "../typedef.h"
#include <memory>
#include "Title/Title.h"
#include "../../../common/network/EventCallBack.h"

namespace MiniProject
{
    enum PlayerState
    {
        DEFAULT_STATE = 0,
        BEAR_STATE = 1,
        DEATH_STATE = 2
    };

    class Player
    {
    public:
        typedef std::shared_ptr<Player> PlayerPtr;

    public:
        Player(PlayerId _pid);
        ~Player(){};

        GameId GetGameId()
        {
            return gid_;
        }

        void setGameId(GameId _gid)
        {
            gid_ = _gid;
        }

        PlayerId PlayId()
        {
            return pid_;
        }

        Title GetTitle()
        {
            return _tt;
        }

        NetId GetNetId()
        {
            return netid_;
        }

        void setNetId(NetId netid)
        {
            netid_ = netid;
        }

        void set_title(Title &t)
        {
            _tt = t;
        }

        void dead();

        void kill();

        void suicide();

        void addbeartime(unsigned long long time);

        void bear();

        void unbear();

        void revive();

        void setPlayerState(PlayerState _st);

        int deadcount()
        {
            return deathCount_;
        }

        int killCount()
        {
            return killCount_;
        }

        void setUserId(UserId _uid)
        {
            userid_ = _uid;
        }

        UserId GetUserId()
        {
            return userid_;
        }

        int suicideCount()
        {
            return suicideCount_;
        }

        int beartime()
        {
            return bearTotalTime_;
        }

        int survivalTime()
        {
            return survivalTotalTime_;
        }

        bool hasTalk()
        {
            return hasFinishedTalk;
        }

        void finishTalk()
        {
            hasFinishedTalk = true;
        }

    private:
        void state_change_callback(PlayerState _oldState, PlayerState _newState);

    private:
        GameId gid_;
        PlayerId pid_;
        UserId userid_;
        Title _tt;
        NetId netid_;
        unsigned int killCount_ = 0;
        unsigned int deathCount_ = 0;
        unsigned int suicideCount_ = 0;
        unsigned int bearTotalTime_ = 0;
        unsigned int survivalTotalTime_ = 0;

        bool hasFinishedTalk = false;

        time_t bearStartTime_;
        time_t survivalTime_;
        PlayerState pstate_ = DEFAULT_STATE;
    };

}

#endif