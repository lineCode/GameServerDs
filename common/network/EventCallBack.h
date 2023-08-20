#ifndef MINIPROJECT_EVENTCALLBACK_HPP
#define MINIPROJECT_EVENTCALLBACK_HPP
#include "google/protobuf/message.h"
#include "type.h"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace MiniProject
{
    using namespace google::protobuf;

    typedef int EventId;

    typedef int NetId;
    typedef int CmdId;
    
    typedef std::shared_ptr<Message> MsgPtr;
    typedef std::shared_ptr<std::string> MessagePtr;

    // typedef void (*EventFunc)(conv_id,std::shared_ptr<std::string> msg);
    typedef std::function<void(const NetId &, MessagePtr)> EventFunc;

    enum MsgType
    {
        MT_RECVMSG = 0,
        MT_CONNECTED = 1,
        MT_DISCONNECTED = 2
    };

    typedef std::function<void(MsgType, NetId, CmdId, MessagePtr)> SessionCallBack;

} // namespace MiniProject

#endif