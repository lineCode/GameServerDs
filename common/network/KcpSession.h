#ifndef MINIPROJECT_KCPSESSION_HPP
#define MINIPROJECT_KCPSESSION_HPP

#include "Session.hpp"
#include <boost/asio.hpp>
#include "../include/ikcp.h"

namespace MiniProject
{
    using namespace boost::asio::ip;

    class KcpSession : public Session
    {
    public:
        KcpSession(NetId _id, std::shared_ptr<udp::socket> sock, SessionCallBack _cb) : Session(_id, SessionType::UDP_SESSION, _cb), socket_(sock)
        {
            init_kcp();
        };
        KcpSession(NetId _id, std::shared_ptr<udp::socket> sock, SessionCallBack _cb, ConnectType _type) : Session(_id, SessionType::UDP_SESSION, _cb, _type), socket_(sock)
        {
            init_kcp();
        }

        ~KcpSession();

        void init_kcp();

        void set_udp_remote_endpoint(const udp::endpoint &_ep);

        void set_socket(udp::socket &sock);

        void send_msg(MessagePtr ptr);

        void start();
        void stop();

        void input(const std::string &data);

        void update(uint32_t clock);

        void send_udp_package(const std::string &data);

    private:
        static int udp_output(const char *buf, int len, struct IKCPCB *kcp, void *user);

        std::shared_ptr<udp::socket> socket_;

        udp::endpoint endpoint_;

        struct IKCPCB *p_kcp_; // --own

        bool isStop;
    };
}

#endif