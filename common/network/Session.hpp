#ifndef MINIPROJECT_Session_HPP
#define MINIPROJECT_Session_HPP

#include "type.h"
#include <memory>
#include "EventCallBack.h"
#include "../include/codec.h"

namespace MiniProject
{
    enum SessionType
    {
        DEFAULT_SESSION = 0,
        UDP_SESSION = 1,
        TCP_SESSION = 2
    };

    class Session
    {
    public:
        typedef std::shared_ptr<Session> SessionPtr;

    public:
        Session(NetId _id) : netid_(_id){};
        Session(NetId _id, SessionType _type, SessionCallBack _cb) : netid_(_id), sessionType_(_type), sessionCb_(_cb){};
        Session(NetId _id, SessionType _type, SessionCallBack _cb, ConnectType _cttype) : netid_(_id), sessionType_(_type), sessionCb_(_cb), type_(_cttype){};
        virtual ~Session(){};

    public:
        virtual void start(){};
        virtual void stop(){};

        virtual void input(const std::string &data){};
        virtual void update(uint32_t cur_clock_){};

        virtual NetId GetNetId()
        {
            return netid_;
        }

        void set_uid(uint32_t _uid)
        {
            uid_ = _uid;
        }

        uint32_t GetUid()
        {
            return uid_;
        }

        virtual void send_msg(MessagePtr ptr) = 0;

        void set_nettype(ConnectType _type)
        {
            type_ = _type;
        }

        void set_message_callback(SessionCallBack _cb)
        {
            sessionCb_ = _cb;
        }

        ConnectType GetNetType()
        {
            return type_;
        }

        bool isTimeOut()
        {
            return false;
        }

    protected:
        virtual void recv(){};

        void call_msg_callback(MsgType _type, int cmd_id, std::shared_ptr<std::string> msg)
        {
            if (sessionCb_)
            {
                sessionCb_(_type, netid_, cmd_id, msg);
            }
        }

    protected:
        SessionType sessionType_ = SessionType::DEFAULT_SESSION;
        ConnectType type_ = ConnectType::CT_DEFAULT;
        bool stop_;
        NetId netid_;
        uint32_t uid_ = 0; // 服务器专属的uid号

        ProtoCodec codec;
        // 回调事件
        SessionCallBack sessionCb_;
        uint64_t last_packet_recv_time_;
    };

}
#endif