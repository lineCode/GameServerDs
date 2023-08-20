#ifndef MINIPROJECT_GATESERVER_HPP
#define MINIPROJECT_GATESERVER_HPP

#include "../../common/network/server.h"

namespace MiniProject
{
    class GateServer : public Server, boost::noncopyable
    {
    public:
        static GateServer *Instance()
        {
            if (instance_ == nullptr)
            {
                std::lock_guard<std::mutex> lock_(instanceMutex_);
                if (instance_ == nullptr)
                {
                    instance_ = new GateServer();
                }
            }
            return instance_;
        }

    private:
        void onConnected(NetId _netID);
        void onDisConnected(NetId _netID);
        void initcallback();

    private:
        static GateServer *instance_;
        static std::mutex instanceMutex_;
    };
}

#endif