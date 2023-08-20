#ifndef MINIPROJECT_SESSION_MANAGER_HPP
#define MINIPROJECT_SESSION_MANAGER_HPP

#include "Session.hpp"
#include <list>
#include <mutex>
#include <unordered_map>
#include <boost/noncopyable.hpp>

namespace MiniProject
{

    class SessionManager : public boost::noncopyable
    {
    public:
        typedef std::shared_ptr<SessionManager> SessionManagerPtr;

    public:
        SessionManager();
        ~SessionManager();

        Session::SessionPtr findSessionByType(ConnectType _ctp);

        Session::SessionPtr findSessionByNetId(NetId _id);

        Session::SessionPtr findSessionByUid(const uint32_t &_uid);

        void updateSessionType(NetId _id, ConnectType _type);

        void updateSessionUiD(NetId _id, uint32_t uid);

        void addSessionById(NetId _id, Session::SessionPtr _ptr);

        void removeSessionPById(NetId _id);

        void sendMsgByType(ConnectType _ctp, MessagePtr _msgPtr);

        void sendMsgByNetId(NetId _id, MessagePtr _msgPtr);

        static NetId GetNewKcpConv()
        {
            static uint32_t static_cur_conv = 100000;
            static_cur_conv++;
            return static_cur_conv;
        }

        NetId GetNewTcpId();

        NetId GetNewKcpId();

        uint16_t GetNetId();

    private:
        std::list<Session::SessionPtr> sessionLists_;
        std::unordered_map<NetId, std::list<Session::SessionPtr>::iterator> sessionMaps_;
        std::unordered_map<ConnectType, std::list<std::list<Session::SessionPtr>::iterator>> sessionTypeMaps_;
        std::unordered_map<uint32_t, std::list<Session::SessionPtr>::iterator> sessionUidMaps_;
        std::mutex sessionMutex_;

        uint16_t current_id = 0;
        std::mutex idMutex_;

        static const uint16_t TCP_MAGIC_NUMBER;
        static const uint16_t UDP_MAGIC_NUMBER;
    };
}

#endif