#include "ConnectionManager.h"
#include <cassert>
namespace MiniProject
{
    void ConnectionManager::addConnectionPtr(int netid, Connection::ConnectionPtr _ptr)
    {
        std::lock_guard<std::mutex> lock(connectionMapMutex_);

        if (netid <= 0)
        {
            return;
        }

        auto it = connectionMap_.find(netid);

        if (it != connectionMap_.end())
        {
            return;
        }

        connectionMap_[netid] = _ptr;
    }

    void ConnectionManager::removeConnectionPtr(int netId)
    {
        std::lock_guard<std::mutex> lock(connectionMapMutex_);

        auto it = connectionMap_.find(netId);

        if (it != connectionMap_.end())
        {
            connectionMap_.erase(it);
        }
    }

    Connection::ConnectionPtr ConnectionManager::findConnectionByNetId(int netId)
    {
        std::lock_guard<std::mutex> lock(connectionMapMutex_);

        auto it = connectionMap_.find(netId);

        if (it != connectionMap_.end())
        {
            return it->second;
        }

        return Connection::ConnectionPtr();
    }

}
