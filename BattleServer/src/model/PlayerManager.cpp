#include "PlayerManager.h"

namespace MiniProject
{

    GameId PlayerManager::findGameByUser(const UserId &uid)
    {
        std::lock_guard<std::mutex> lock(mapMutex);

        auto it = userMaps.find(uid);

        if (it == userMaps.end())
        {
            return 0;
        }
        return it->second;
    }

    GameId PlayerManager::findGameByNet(const NetId &netid)
    {
        std::lock_guard<std::mutex> lock(mapMutex);

        auto it = netMaps.find(netid);

        if (it == netMaps.end())
        {
            return 0;
        }

        return it->second;
    }

    void PlayerManager::addGameByUser(const UserId &uid,const  GameId &gid)
    {
        std::lock_guard<std::mutex> lock(mapMutex);

        userMaps[uid] = gid;
    }

    void PlayerManager::addGameByNet(const NetId &uid, const GameId &gid)
    {
        std::lock_guard<std::mutex> lock(mapMutex);

        netMaps[uid] = gid;
    }

}