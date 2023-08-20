#ifndef MINIPROJECT_ROOMSERVER_HPP
#define MINIPROJECT_ROOMSERVER_HPP

#include "../../common/network/server.h"

namespace MiniProject
{
    class RoomServer : public Server, boost::noncopyable
    {
    public:
        static RoomServer *Instance()
        {
            static RoomServer _server;

            return &_server;
        }

        


    private:
        void onConnected(NetId _netID);
        void onDisConnected(NetId _netID);
        void initcallback();
    };
}

#endif