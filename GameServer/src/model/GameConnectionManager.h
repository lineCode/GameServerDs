#ifndef MINIPROJECT_GS_CONNECTIONMANAGER_HPP
#define MINIPROJECT_GS_CONNECTIONMANAGER_HPP

#include "../../../common/network/EventCallBack.h"
#include "typedef.h"
#include <boost/noncopyable.hpp>
#include <mutex>
#include <unordered_map>

namespace MiniProject
{
    class GameConnectionManager : public boost::noncopyable
    {
    public:
        static GameConnectionManager *Instance()
        {
            static GameConnectionManager mananger;
            return &mananger;
        }

        void addUser(NetId netId, UserId _uid);
        void removeUser(NetId netId, UserId _uid);
        void removeNet(NetId netId);
        std::unordered_set<UserId> GetUsers(NetId _netid);

    private:
        std::unordered_map<NetId, std::unordered_set<UserId>> userMaps;
        std::mutex mapLock;
    };

} // namespace MiniProject

#endif