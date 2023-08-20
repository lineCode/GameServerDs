#ifndef MINIPROJECT_PLAYERMANAGER_HPP
#define MINIPROJECT_PLAYERMANAGER_HPP

#include "typedef.h"
#include <unordered_map>
#include <boost/noncopyable.hpp>
#include <mutex>
#include "../../../common/network/EventCallBack.h"

namespace MiniProject
{
    class PlayerManager : public boost::noncopyable
    {
    public:
        static PlayerManager *Instance()
        {
            static PlayerManager manager;
            return &manager;
        }

        GameId findGameByUser(const UserId &uid);
        GameId findGameByNet(const NetId &netid);

        void addGameByUser(const UserId &uid,const GameId &gid);
        void addGameByNet(const NetId &uid,const GameId &gid);

    private:
        std::unordered_map<UserId, GameId> userMaps;
        std::unordered_map<NetId, GameId> netMaps;
        std::mutex mapMutex;
    };
}

#endif