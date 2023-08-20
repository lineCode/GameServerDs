#ifndef MINIPROJECT_ROOM_ROOMMANAGER_HPP
#define MINIPROJECT_ROOM_ROOMMANAGER_HPP

#include "Room.h"
#include <memory>
#include <boost/noncopyable.hpp>
#include <unordered_map>
#include <list>
#include <unordered_set>
#include <mutex>
#include <atomic>

namespace MiniProject
{
    // 根据房间状态做进一步的区分？或者在这里不考虑，匹配服上完成相应的逻辑
    class RoomManager : public boost::noncopyable
    {
    public:
        static RoomManager *Instance()
        {
            static RoomManager _rManager;
            return &_rManager;
        }

        RoomId GenerateRoomID(uint32_t _uid);
        Room::RoomPtr findRoomByRoomId(RoomId _rId);
        Room::RoomPtr findRoomByType(int _type);

        bool findRoomForCreate(RoomId _rId, Room::RoomPtr _ptr);

        bool AddRoom(RoomId _rID, Room::RoomPtr _ptr);
        void RemoveRoom(RoomId _rID);
        void SafeRemoveRoom(RoomId _rID);

        std::list<Room::RoomPtr> GetAllRooms()
        {
            std::lock_guard<std::mutex> lock_(m_mutex_rooms_);
            return m_list_id_rooms_;
        }

    private:
        std::list<Room::RoomPtr> m_list_id_rooms_;                                                // 存储房间的实体
        std::unordered_map<RoomId, std::list<Room::RoomPtr>::iterator> m_map_id_rooms_;           // 根据房间id索引房间
        std::unordered_map<int, std::list<std::list<Room::RoomPtr>::iterator>> m_map_type_rooms_; // 根据房间人数类型索引房间

        std::mutex m_mutex_rooms_;

        std::atomic<uint32_t> m_cur_roomids_{0};
    };
}

#endif