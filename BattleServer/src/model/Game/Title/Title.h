#ifndef MINIPROJECT_TITLE_HPP
#define MINIPROJECT_TITLE_HPP

#include <string>
#include <functional>
#include <queue>
#include "TitleConfig.h"

namespace MiniProject
{
    typedef std::function<bool()> SettleFunc;

    class Game;
    class Player;

    class TitleManager
    {
    public:
        TitleManager(Game *game, Player *player);

        Title GetTitle(int level);

        std::priority_queue<Title> GetLevel(int level);

    public:
        bool OneLifePass();
        bool LongestExistence();
        bool MostKilled();
        bool NoKilled();
        bool MostBeared();
        bool BearLeast();
        bool MostDeath();
        bool Traveler();

    private:
        Game *_gameptr;
        Player *_playptr;
        std::unordered_map<std::string, SettleFunc> funcMaps;
    };

}

#endif