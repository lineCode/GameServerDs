#include "GameConnectionManager.h"

namespace MiniProject
{

    void GameConnectionManager::addUser(NetId netId, UserId _uid)
    {
        std::lock_guard<std::mutex> lock(mapLock);

        auto it = userMaps.find(netId);

        if (it == userMaps.end())
        {
            userMaps[netId] = std::unordered_set<UserId>();
        }
        userMaps[netId].insert(_uid);
    }

    void GameConnectionManager::removeUser(NetId netId, UserId _uid)
    {
        std::lock_guard<std::mutex> lock(mapLock);

        auto it = userMaps.find(netId);

        if (it == userMaps.end())
        {
            return;
        }
        if (it->second.count(_uid))
        {
            it->second.erase(_uid);
        }
    }

    void GameConnectionManager::removeNet(NetId netId)
    {
        std::lock_guard<std::mutex> lock(mapLock);

        auto it = userMaps.find(netId);

        if (it != userMaps.end())
        {
            userMaps.erase(it);
        }
    }

    std::unordered_set<UserId> GameConnectionManager::GetUsers(NetId _netid)
    {
        std::lock_guard<std::mutex> lock(mapLock);

        auto it = userMaps.find(_netid);

        if (it == userMaps.end())
        {
            return std::unordered_set<UserId>();
        }

        return it->second;
    }

} // namespace MiniProject