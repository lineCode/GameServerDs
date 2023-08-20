#include "RoomManager.h"
#include <assert.h>
#include <chrono>

namespace MiniProject
{

    Room::RoomPtr RoomManager::findRoomByRoomId(RoomId _rId)
    {
        std::lock_guard<std::mutex> lock_(m_mutex_rooms_);

        auto it = m_map_id_rooms_.find(_rId);

        if (it != m_map_id_rooms_.end())
        {
            return *(it->second);
        }

        return Room::RoomPtr();
    }

    Room::RoomPtr RoomManager::findRoomByType(int _type)
    {
        std::lock_guard<std::mutex> lock_(m_mutex_rooms_);

        auto it = m_map_type_rooms_.find(_type);

        if (it != m_map_type_rooms_.end() && (it->second).size() > 0)
        {
            return *(it->second.front());
        }

        return Room::RoomPtr();
    }

    bool RoomManager::findRoomForCreate(RoomId _rId, Room::RoomPtr _ptr)
    {
        std::lock_guard<std::mutex> lock_(m_mutex_rooms_);

        auto it = m_map_id_rooms_.find(_rId);

        if (it != m_map_id_rooms_.end())
        {
            return false;
        }

        m_list_id_rooms_.push_back(_ptr);
        m_map_id_rooms_[_rId] = std::prev(m_list_id_rooms_.end());

        auto iter = m_map_type_rooms_.find(_ptr->MAX_USER_COUNT());

        if (iter != m_map_type_rooms_.end())
        {
            iter->second.push_back(std::prev(m_list_id_rooms_.end()));
        }
        else
        {
            m_map_type_rooms_[_ptr->MAX_USER_COUNT()] = std::list<std::list<Room::RoomPtr>::iterator>{std::prev(m_list_id_rooms_.end())};
        }
        return true;
    }

    bool RoomManager::AddRoom(RoomId _rID, Room::RoomPtr _ptr)
    {
        std::lock_guard<std::mutex> lock_(m_mutex_rooms_);

        if (m_map_id_rooms_.count(_rID))
        {
            return false;
        }

        m_list_id_rooms_.push_back(_ptr);
        m_map_id_rooms_[_rID] = std::prev(m_list_id_rooms_.end());

        int count = _ptr->MAX_USER_COUNT();

        auto it = m_map_type_rooms_.find(count);

        if (it == m_map_type_rooms_.end())
        {
            m_map_type_rooms_[count] = std::list<std::list<Room::RoomPtr>::iterator>{std::prev(m_list_id_rooms_.end())};
        }
        else
        {
            it->second.push_front(std::prev(m_list_id_rooms_.end()));
        }

        return true;
    }

    void RoomManager::RemoveRoom(RoomId _rID)
    {
        std::lock_guard<std::mutex> lock_(m_mutex_rooms_);

        auto it = m_map_id_rooms_.find(_rID);

        if (it == m_map_id_rooms_.end())
        {
            return;
        }

        int _count = (*(it->second))->MAX_USER_COUNT();

        auto _its = m_map_type_rooms_.find(_count);

        assert(_its != m_map_type_rooms_.end());

        std::list<std::list<Room::RoomPtr>::iterator> &_iters = _its->second;

        for (std::list<std::list<Room::RoomPtr>::iterator>::iterator _listiter = _iters.begin(); _listiter != _iters.end(); _listiter++)
        {
            if ((*(*_listiter))->GetRoomId() == _rID)
            {
                _listiter = _iters.erase(_listiter);
                break;
            }
        }
        m_list_id_rooms_.erase(it->second);
        m_map_id_rooms_.erase(it);
    }

    void RoomManager::SafeRemoveRoom(RoomId _rID)
    {
        std::lock_guard<std::mutex> lock_(m_mutex_rooms_);

        auto it = m_map_id_rooms_.find(_rID);

        if (it == m_map_id_rooms_.end())
        {
            return;
        }

        if ((*(it->second))->cur_user_count() > 0)
        {
            return;
        }

        int _count = (*(it->second))->MAX_USER_COUNT();

        auto _its = m_map_type_rooms_.find(_count);

        assert(_its != m_map_type_rooms_.end());

        std::list<std::list<Room::RoomPtr>::iterator> &_iters = _its->second;

        for (std::list<std::list<Room::RoomPtr>::iterator>::iterator _listiter = _iters.begin(); _listiter != _iters.end(); _listiter++)
        {
            if ((*(*_listiter))->GetRoomId() == _rID)
            {
                _listiter = _iters.erase(_listiter);
                break;
            }
        }
        m_list_id_rooms_.erase(it->second);
        m_map_id_rooms_.erase(it);
    }

    RoomId RoomManager::GenerateRoomID(uint32_t uid)
    {
        // RoomId _id = 0;

        // uint64_t _addr = addr;
        // uint32_t _port = port;
        // _id |= (_addr << 32);
        // _id |= (_port << 16);

        // std::lock_guard<std::mutex> lock(mutexIds);
        // uint16_t rid = m_cur_roomids_++;

        // return _id |= rid;

        RoomId _id = 0;

        uint64_t _uid = uid;

        _id |= (_uid << 32);

        uint32_t rid = m_cur_roomids_.fetch_add(1);

        return _id |= rid;
    }

}