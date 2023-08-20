#ifndef MINIPROJECT_NODE_DBHANDLE_HPP
#define MINIPROJECT_NODE_DBHANDLE_HPP

#include <string>
#include "../../../common/network/EventCallBack.h"
#include "../model/NodeTypedef.h"
#include <google/protobuf/message.h>

namespace MiniProject
{
    const std::string ROOM_PREFIX = "ROOMMSG::";

    class DbHandle
    {
    public:
        static bool CreateRoomInfo(const RoomId &roomId, MsgPtr _ptr, int expire_time = 5);

        static MsgPtr GetRoomInfoCache(const RoomId &roomId);

        static bool ExtendRoomCacheTime(const RoomId &roomId, int expire_time = 5);
    };

}

#endif
