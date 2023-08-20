#include "Player.h"

namespace MiniProject
{
    Player::Player(PlayerId _pid) : pid_(_pid){};

    void Player::bear()
    {
        setPlayerState(BEAR_STATE);
    }

    void Player::unbear()
    {
        setPlayerState(DEFAULT_STATE);
    }

    void Player::dead()
    {
        ++deathCount_;
    }

    void Player::kill()
    {
        ++killCount_;
    }

    void Player::suicide()
    {
        ++suicideCount_;
        ++deathCount_;
    }

    void Player::addbeartime(unsigned long long time)
    {
        bearTotalTime_ += time;
    }

    void Player::revive()
    {
        setPlayerState(DEFAULT_STATE);
    }

    void Player::setPlayerState(PlayerState _st)
    {
        if (pstate_ == _st)
        {
            return;
        }
        PlayerState _oldSt = pstate_;
        pstate_ = _st;
        state_change_callback(_oldSt, _st);
    }

    void Player::state_change_callback(PlayerState _oldState, PlayerState _newState)
    {
        if (_oldState == DEFAULT_STATE)
        {
            if (_newState == DEATH_STATE)
            {
                ++deathCount_;
            }
            else if (_newState == BEAR_STATE)
            {
                bearStartTime_ = time(nullptr);
            }
        }
        else if (_oldState == BEAR_STATE)
        {
            if (_newState == DEATH_STATE)
            {
                ++deathCount_;
                bearTotalTime_ += time(nullptr) - bearStartTime_;
                survivalTotalTime_ = time(nullptr) - survivalTime_;
            }
            else if (_newState == DEFAULT_STATE)
            {
                bearTotalTime_ += time(nullptr) - bearStartTime_;
            }
        }
        else if (_oldState == DEATH_STATE)
        {
            if (_newState == DEFAULT_STATE)
            {
                survivalTime_ = time(nullptr);
            }
        }
    }

}