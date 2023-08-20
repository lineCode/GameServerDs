#include "SessionManager.h"
#include <cassert>

namespace MiniProject
{

    const uint16_t SessionManager::TCP_MAGIC_NUMBER = 1;
    const uint16_t SessionManager::UDP_MAGIC_NUMBER = 0;

    SessionManager::SessionManager() {}

    SessionManager::~SessionManager() {}

    Session::SessionPtr SessionManager::findSessionByType(ConnectType _ctp)
    {
        std::lock_guard<std::mutex> lock_(sessionMutex_);

        for (Session::SessionPtr &_ptr : sessionLists_)
        {
            if (_ptr->GetNetType() == _ctp)
            {
                return _ptr;
            }
        }
        return Session::SessionPtr();
    }

    Session::SessionPtr SessionManager::findSessionByNetId(NetId _id)
    {
        std::lock_guard<std::mutex> lock_(sessionMutex_);

        auto it = sessionMaps_.find(_id);

        if (it == sessionMaps_.end())
        {
            return Session::SessionPtr();
        }

        return *(it->second);
    }

    Session::SessionPtr SessionManager::findSessionByUid(const uint32_t &_uid)
    {
        std::lock_guard<std::mutex> lock_(sessionMutex_);

        auto it = sessionUidMaps_.find(_uid);

        if (it == sessionUidMaps_.end())
        {
            return Session::SessionPtr();
        }

        return *(it->second);
    }

    void SessionManager::addSessionById(NetId _id, Session::SessionPtr _ptr)
    {
        if (!_ptr || _id != _ptr->GetNetId())
        {
            return;
        }

        std::lock_guard<std::mutex> lock_(sessionMutex_);

        auto it = sessionMaps_.find(_id);

        if (it != sessionMaps_.end())
        {
            return;
        }
        sessionLists_.push_back(_ptr);
        sessionMaps_[_id] = std::prev(sessionLists_.end());

        ConnectType _type = _ptr->GetNetType();

        if (_type != ConnectType::CT_DEFAULT)
        {
            auto listIter = sessionTypeMaps_.find(_type);

            if (listIter == sessionTypeMaps_.end())
            {
                sessionTypeMaps_[_type] = std::list<std::list<Session::SessionPtr>::iterator>();
            }
            sessionTypeMaps_[_type].push_back(std::prev(sessionLists_.end()));
        }
    }

    void SessionManager::removeSessionPById(NetId _id)
    {
        std::lock_guard<std::mutex> lock_(sessionMutex_);

        auto it = sessionMaps_.find(_id);

        if (it != sessionMaps_.end())
        {
            ConnectType _type = (*it->second)->GetNetType();

            if (_type != ConnectType::CT_DEFAULT)
            {
                std::list<std::list<Session::SessionPtr>::iterator> &typeLists = sessionTypeMaps_[_type];
                for (auto _listIter = typeLists.begin(); _listIter != typeLists.end(); ++_listIter)
                {
                    if (((*_listIter)) == it->second)
                    {
                        typeLists.erase(_listIter);
                        break;
                    }
                }
                if (typeLists.empty())
                {
                    sessionTypeMaps_.erase((*it->second)->GetNetType());
                }
            }

            uint32_t _uid = (*it->second)->GetUid();

            if (_uid != 0)
            {
                sessionUidMaps_.erase(_uid);
            }

            sessionLists_.erase(it->second);
            sessionMaps_.erase(it);
        }
    }

    void SessionManager::sendMsgByType(ConnectType _ctp, MessagePtr _msgPtr)
    {
        Session::SessionPtr _ptr = findSessionByType(_ctp);

        if (_ptr)
        {
            _ptr->send_msg(_msgPtr);
        }
    }

    void SessionManager::sendMsgByNetId(NetId _id, MessagePtr _msgPtr)
    {
        Session::SessionPtr _ptr = findSessionByNetId(_id);

        if (_ptr)
        {
            _ptr->send_msg(_msgPtr);
        }
    }

    void SessionManager::updateSessionType(NetId _id, ConnectType _type)
    {
        std::lock_guard<std::mutex> lock_(sessionMutex_);

        auto it = sessionMaps_.find(_id);

        if (it == sessionMaps_.end())
        {
            return;
        }

        Session::SessionPtr _ptr = (*it->second);

        if (_ptr->GetNetType() == _type)
        {
            return;
        }

        if (_ptr->GetNetType() != ConnectType::CT_DEFAULT)
        {
            // 数据一致时保证该key存在，不做检查
            std::list<std::list<Session::SessionPtr>::iterator> &typeLists = sessionTypeMaps_[_ptr->GetNetType()];
            for (auto _listIter = typeLists.begin(); _listIter != typeLists.end(); ++_listIter)
            {
                if ((*(*_listIter))->GetNetId() == _ptr->GetNetId())
                {
                    typeLists.erase(_listIter);
                    break;
                }
            }
            if (typeLists.empty())
            {
                sessionTypeMaps_.erase(_ptr->GetNetType());
            }
        }

        if (_type != ConnectType::CT_DEFAULT)
        {
            auto typeIter = sessionTypeMaps_.find(_type);
            if (typeIter == sessionTypeMaps_.end())
            {
                sessionTypeMaps_[_type] = std::list<std::list<Session::SessionPtr>::iterator>();
            }
            sessionTypeMaps_[_type].push_back(it->second);
        }
        _ptr->set_nettype(_type);
    }

    void SessionManager::updateSessionUiD(NetId _id, uint32_t uid)
    {
        std::lock_guard<std::mutex> Lock(sessionMutex_);

        auto it = sessionMaps_.find(_id);

        if (it == sessionMaps_.end())
        {
            return;
        }

        Session::SessionPtr _ptr = (*it->second);

        uint32_t oldUid = _ptr->GetUid();

        if (oldUid == uid)
        {
            return;
        }

        if (oldUid != 0)
        {
            sessionUidMaps_.erase(oldUid);
        }

        if (uid != 0)
        {
            sessionUidMaps_[uid] = it->second;
        }

        _ptr->set_uid(uid);
    }

    uint16_t SessionManager::GetNetId()
    {
        std::lock_guard<std::mutex> lock(idMutex_);
        return current_id++;
    }

    NetId SessionManager::GetNewTcpId()
    {
        uint16_t _id = GetNetId();

        return (NetId)((uint32_t)(TCP_MAGIC_NUMBER << 16) | (uint32_t)(_id));
    }

    NetId SessionManager::GetNewKcpId()
    {
        uint16_t _id = GetNetId();

        return (NetId)((uint32_t)(UDP_MAGIC_NUMBER << 16) | (uint32_t)(_id));
    }

}