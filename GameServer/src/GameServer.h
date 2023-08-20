#ifndef MINPROJECT_GAMESERVER_HPP
#define MINPROJECT_GAMESERVER_HPP
#include "../../common/network/server.h"
#include "model/typedef.h"
#include <boost/noncopyable.hpp>
#include "model/User/UserManager.h"

namespace MiniProject
{
    class GameServer : public Server, boost::noncopyable
    {
    public:
        static GameServer *Instance()
        {
            static GameServer _server;
            return &_server;
        }

        uint64_t GetAccountId(const UserId &_userid);
        UserId ParseIdFromIdProxy(uint64_t _userProxy);
        void userDisConnected(User::UserPtr uptr);

    private:
        void onConnected(NetId _netID);
        void onDisConnected(NetId _netID);
        void initcallback();

        void onGateServerDisConnected(NetId _netId);
        void onNodeServerDisConnected(NetId _netId);

    };

}

#endif