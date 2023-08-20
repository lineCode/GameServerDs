#ifndef MINIPROJECT_BATTLESERVER_HPP
#define MINIPROJECT_BATTLESERVER_HPP

#include <boost/noncopyable.hpp>
#include "../../common/network/server.h"
#include "model/typedef.h"
#include "model/Game/Game.h"

namespace MiniProject
{
    class BattleServer : public Server, boost::noncopyable
    {
    public:
        static BattleServer *Instance()
        {
            static BattleServer _server;
            return &_server;
        }

        void StartGame(Game::GamePtr _ptr);
        void PauseGame(Game::GamePtr _ptr);
        void StopGame(Game::GamePtr _ptr);

    private:
        void onConnected(NetId _netID);
        void onDisConnected(NetId _netID);
        void initcallback();
    };

}

#endif