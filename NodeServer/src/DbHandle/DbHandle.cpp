#include "DbHandle.h"
#include "../../../common/protobuf/node_msg.pb.h"
#include "spdlog/spdlog.h"
#include "../../../common/datahandle/redisPool.h"
#include "thread"
#include <atomic>

namespace MiniProject
{
    bool DbHandle::CreateRoomInfo(const RoomId &RoomId, MsgPtr _ptr, int expire_time)
    {
        spdlog::info("bool DbHandle::CacheRoomInfo() {}", RoomId);

        std::string _key = ROOM_PREFIX + std::to_string(RoomId);

        // 加入分布式锁，否则存在问题
        RedisConnectionRaII redisRaII(RedisConnectionPool::GetInstance());

        redisRaII.lock(ROOM_PREFIX);
        // ((pb::RoomMsgCacheInfo *)(_ptr.get()))->set_host(std::to_string(*_threadid));

        // 下面两个步骤需要保证原子性
        std::string _val;
        if (redisRaII.get(_key, _val) == 1)
        {
            return false;
        }
        _val = ((pb::RoomMsgCacheInfo *)(_ptr.get()))->SerializeAsString();
        return redisRaII.set(_key, _val, expire_time) == 1;
    }

    MsgPtr DbHandle::GetRoomInfoCache(const RoomId &RoomId)
    {
        spdlog::info("MsgPtr DbHandle::GetRoomInfoCache {}", RoomId);

        std::string _key = ROOM_PREFIX + std::to_string(RoomId);

        // 加入分布式锁，否则存在问题
        RedisConnectionRaII redisRaII(RedisConnectionPool::GetInstance());

        redisRaII.lock(ROOM_PREFIX);

        std::string _val;
        if (redisRaII.get(_key, _val) != 1)
        {
            return MsgPtr();
        }

        MsgPtr _rspptr;
        ((pb::RoomMsgCacheInfo *)(_rspptr.get()))->ParseFromString(_val);
        return _rspptr;
    }

    bool DbHandle::ExtendRoomCacheTime(const RoomId &RoomId, int expire_time)
    {
        spdlog::info("bool DbHandle::ExtenRoomCacheTime {}", RoomId);

        if (expire_time <= 0)
        {
            return false;
        }

        std::string _key = ROOM_PREFIX + std::to_string(RoomId);

        RedisConnectionRaII redisRaII(RedisConnectionPool::GetInstance());

        redisRaII.lock(ROOM_PREFIX);

        return redisRaII.expire(_key, expire_time) == 1;
    }

}