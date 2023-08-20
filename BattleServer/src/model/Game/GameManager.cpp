#include "GameManager.h"

namespace MiniProject
{

    Game::GamePtr GameManager::find(GameId gid)
    {
        std::lock_guard<std::mutex> lock(gameLock);

        auto it = gameMaps.find(gid);

        if (it == gameMaps.end())
        {
            return Game::GamePtr();
        }
        return it->second;
    }

    bool GameManager::add(Game::GamePtr ptr)
    {
        std::lock_guard<std::mutex> lock(gameLock);
        auto gameit = gameMaps.find(ptr->GetGameId());

        if (gameit != gameMaps.end())
        {
            return false;
        }
        gameMaps[ptr->GetGameId()] = ptr;
        return true;
    }

    std::list<Game::GamePtr> GameManager::GetAllGame()
    {
        std::list<Game::GamePtr> gameLists;

        {
            std::lock_guard<std::mutex> lock(gameLock);
            for (auto it : gameMaps)
            {
                gameLists.push_back(it.second);
            }
        }

        return gameLists;
    }

    bool GameManager::remove(GameId gid)
    {
        std::lock_guard<std::mutex> lock(gameLock);

        auto it = gameMaps.find(gid);

        if (it == gameMaps.end())
        {
            return false;
        }
        gameMaps.erase(it);
        return true;
    }

}