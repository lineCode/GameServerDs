#ifndef MINIPROJECT_SESSION_HPP
#define MINIPROJECT_SESSION_HPP

#include "../include/buffer.h"
#include "../include/codec.h"
#include "EventCallBack.h"
#include "Session.hpp"
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <memory>

namespace MiniProject
{
    using namespace boost::asio::ip;

    class TcpSession : public Session, boost::noncopyable
    {
    public:
        typedef std::shared_ptr<Session> SessionPtr;
        typedef std::shared_ptr<tcp::socket> SockPtr;

    public:
        TcpSession(NetId netid, tcp::socket &_sock, SessionCallBack _cb);
        TcpSession(NetId netid, tcp::socket &_sock, SessionCallBack _cb, ConnectType _type);
        ~TcpSession();

        void send_msg(MessagePtr ptr);

        void start();

        void stop();

        SockPtr GetSocketPtr()
        {
            return sock_;
        }

    private:
        void recv();

    private:
        SockPtr sock_;
        bool stop_;
        TcpBuffer::BufferPtr m_read_buffer_;
        MsgParseState m_st_msg_parse_ = MsgParseState::MSG_NONE;
        std::vector<char> buf;
    };

}

#endif