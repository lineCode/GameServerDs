#ifndef MINIPROJECT_NODESERVER_HPP
#define MINIPROJECT_NODESERVER_HPP

#include "../../common/network/server.h"

namespace MiniProject
{
    class NodeServer : public Server, boost::noncopyable
    {
    public:
        static NodeServer *Instance()
        {
            static NodeServer _server;
            return &_server;
        }


    private:
        void onConnected(NetId _netID);
        void onDisConnected(NetId _netID);
        void initcallback();
    };
}

#endif