#ifndef MINIPROJECT_GATESERVER_CONNECTIONMANAGER_HPP
#define MINIPROJECT_GATESERVER_CONNECTIONMANAGER_HPP

#include "Connection.h"
#include <boost/noncopyable.hpp>
#include <list>
#include <mutex>
#include <unordered_map>

#include "Connection.h"

namespace MiniProject
{
    class ConnectionManager : public boost::noncopyable
    {
    public:
        static ConnectionManager *Instance()
        {
            static ConnectionManager _manager;
            return &_manager;
        }

        void addConnectionPtr(int netid, Connection::ConnectionPtr _ptr);
        void removeConnectionPtr(int netId);

        Connection::ConnectionPtr findConnectionByNetId(int netId);

    private:
        std::unordered_map<int, Connection::ConnectionPtr> connectionMap_;
        std::mutex connectionMapMutex_;
    };

} // namespace MiniProject

#endif