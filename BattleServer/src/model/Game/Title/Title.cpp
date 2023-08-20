#include "Title.h"

#include "../Game.h"
#include "../Player.h"
#include "spdlog/spdlog.h"

namespace MiniProject
{
    TitleManager::TitleManager(Game *game, Player *player)
        : _gameptr(game), _playptr(player)
    {
        funcMaps[TitleConfigName().one_life] = std::bind(&TitleManager::OneLifePass, this);
        funcMaps[TitleConfigName().longestexistence] = std::bind(&TitleManager::LongestExistence, this);
        funcMaps[TitleConfigName().killmost] = std::bind(&TitleManager::MostKilled, this);
        funcMaps[TitleConfigName().nokill] = std::bind(&TitleManager::NoKilled, this);
        funcMaps[TitleConfigName().mostbeared] = std::bind(&TitleManager::MostBeared, this);
        funcMaps[TitleConfigName().nobear] = std::bind(&TitleManager::BearLeast, this);
        funcMaps[TitleConfigName().mostdeath] = std::bind(&TitleManager::MostDeath, this);
        funcMaps[TitleConfigName().traveler] = std::bind(&TitleManager::Traveler, this);
    }

    std::priority_queue<Title> TitleManager::GetLevel(int level)
    {
        std::priority_queue<Title, std::vector<Title>, std::less<Title>> tt;

        tt.push(TitleConfig::Instance()->find(TitleConfigName().one_life));
        tt.push(TitleConfig::Instance()->find(TitleConfigName().longestexistence));
        tt.push(TitleConfig::Instance()->find(TitleConfigName().killmost));
        tt.push(TitleConfig::Instance()->find(TitleConfigName().nokill));
        tt.push(TitleConfig::Instance()->find(TitleConfigName().mostbeared));
        tt.push(TitleConfig::Instance()->find(TitleConfigName().nobear));
        tt.push(TitleConfig::Instance()->find(TitleConfigName().mostdeath));
        tt.push(TitleConfig::Instance()->find(TitleConfigName().traveler));

        if (level == 1)
        {
        }
        else if (level == 2)
        {
        }
        else if (level == 3)
        {
        }

        return tt;
    }

    Title TitleManager::GetTitle(int level)
    {
        std::priority_queue<Title> titelQueues = GetLevel(level);
        while (!titelQueues.empty())
        {
            Title tt = titelQueues.top();
            titelQueues.pop();

            if (funcMaps.count(tt.name))
            {
                if (funcMaps[tt.name]())
                {
                    spdlog::info("Player:{} 获得称号：{}",_playptr->PlayId(),tt.name);
                    return tt;
                }
            }
        }
        return Title();
    }

    bool TitleManager::OneLifePass()
    {
        std::cout << "玩家" << _playptr->PlayId() << ":"  << _playptr->deadcount() << std::endl;
        return _playptr->deadcount() == 0;
    }

    bool TitleManager::LongestExistence()
    {
        std::vector<Player::PlayerPtr> players = _gameptr->GetAllPlayers();

        // for (auto player : players)
        // {
        //     if (_playptr->survivalTime() < player->survivalTime())
        //     {
        //         return false;
        //     }
        // }

        for (auto player : players)
        {
            if (_playptr->deadcount() > player->deadcount())
            {
                return false;
            }
        }
        return true;
    }

    bool TitleManager::MostKilled()
    {
        std::vector<Player::PlayerPtr> players = _gameptr->GetAllPlayers();

        for (auto player : players)
        {
            if (_playptr->killCount() < player->killCount())
            {
                return false;
            }
        }
        return _playptr->killCount() > 0;
    }

    bool TitleManager::NoKilled()
    {
        return _playptr->killCount() == 0;
    }

    bool TitleManager::MostBeared()
    {
        std::vector<Player::PlayerPtr> players = _gameptr->GetAllPlayers();

        for (auto player : players)
        {
            if (_playptr->beartime() < player->beartime())
            {
                return false;
            }
        }
        return _playptr->beartime() > 0;
    }

    bool TitleManager::BearLeast()
    {
        std::vector<Player::PlayerPtr> players = _gameptr->GetAllPlayers();

        for (auto player : players)
        {
            if (_playptr->beartime() > player->beartime())
            {
                return false;
            }
        }
        return true;
    }

    bool TitleManager::MostDeath()
    {
        std::vector<Player::PlayerPtr> players = _gameptr->GetAllPlayers();

        for (auto player : players)
        {
            if (_playptr->deadcount() < player->deadcount())
            {
                return false;
            }
        }
        return _playptr->deadcount() > 0;
    }

    bool TitleManager::Traveler()
    {
        return true;
    }

}