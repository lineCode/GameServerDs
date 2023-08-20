#ifndef BATTLESERVER_GAMEMANAGER_HPP
#define BATTLESERVER_GAMEMANAGER_HPP

#include <unordered_map>
#include <mutex>
#include <list>
#include <boost/noncopyable.hpp>
#include "Game.h"
#include "../typedef.h"

namespace MiniProject
{
    class GameManager : public boost::noncopyable
    {
    public:
        static GameManager *Instance()
        {
            static GameManager gameManager;
            return &gameManager;
        }

        std::list<Game::GamePtr> GetAllGame();

        Game::GamePtr find(GameId gid);

        bool add(Game::GamePtr ptr);
        bool remove(GameId gid);

    private:
        std::unordered_map<GameId, Game::GamePtr> gameMaps;
        std::mutex gameLock;
    };

}

#endif