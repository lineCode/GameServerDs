#ifndef MINIPROJECT_LOGINSERVER_HPP
#define MINIPROJECT_LOGINSERVER_HPP

#include "../../common/network/server.h"

namespace MiniProject
{
    class LoginServer : public Server, boost::noncopyable
    {
    public:
        static LoginServer *Instance()
        {
            static LoginServer _server;
            return &_server;
        }

    private:
        void onConnected(NetId _netID);
        void onDisConnected(NetId _netID);
        void initcallback();
    };
}

#endif