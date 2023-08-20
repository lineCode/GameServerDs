#ifndef MINIPROJECT_DBSERVER_HPP
#define MINIPROJECT_DBSERVER_HPP

#include "../../common/network/server.h"
#include <boost/asio.hpp>

namespace MiniProject
{
    class DBServer : public Server, boost::noncopyable
    {
    public:
        static DBServer *Instance()
        {
            static DBServer _server;
            return &_server;
        }

    private:
        void initcallback();
        void onConnected(NetId _netID);
        void onDisConnected(NetId _netID);
    };

}

#endif