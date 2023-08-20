#include "KcpSession.h"
#include "KcpConectionPacket.h"
#include "../include/clock.h"

namespace MiniProject
{

    KcpSession::~KcpSession()
    {
        stop();
    }

    void KcpSession::init_kcp()
    {
        p_kcp_ = ikcp_create(netid_, (void *)this);
        p_kcp_->output = &KcpSession::udp_output;

        // 配置kcp的一些参数量
        ikcp_nodelay(p_kcp_, 1, 5, 1, 1);
    }

    void KcpSession::set_socket(udp::socket &sock)
    {
    }

    void KcpSession::send_udp_package(const std::string &data)
    {
        socket_->send_to(boost::asio::buffer(data), endpoint_);
    }

    int KcpSession::udp_output(const char *buf, int len, struct IKCPCB *kcp, void *user)
    {
        ((KcpSession *)user)->send_udp_package(std::string(buf, len));
        return 0;
    }

    void KcpSession::send_msg(MessagePtr _ptr)
    {
        ikcp_send(p_kcp_, _ptr->c_str(), _ptr->size());
    }

    void KcpSession::input(const std::string &data)
    {
        last_packet_recv_time_ = iclock();
        ikcp_input(p_kcp_, data.c_str(), data.size());
        while (true)
        {
            char kcp_buf[1024 * 1000] = "";
            int kcp_recvd_bytes = ikcp_recv(p_kcp_, kcp_buf, sizeof(kcp_buf));
            if (kcp_recvd_bytes <= 0)
            {
                break;
            }
            else
            {
                std::string package(kcp_buf, kcp_recvd_bytes);
                if (codec.decode(package.c_str(), package.size()) == 0)
                {
                    call_msg_callback(MsgType::MT_RECVMSG, codec.cmd_id(), std::make_shared<std::string>(codec.data()));
                }
            }
        }
    }

    void KcpSession::update(uint32_t clock)
    {
        ikcp_update(p_kcp_, clock);
    }

    void KcpSession::set_udp_remote_endpoint(const udp::endpoint &_ep)
    {
        endpoint_ = _ep;
    }

    void KcpSession::start()
    {
        isStop = false;
    }

    void KcpSession::stop()
    {
        if (isStop)
        {
            return;
        }
        isStop = true;
        std::string disconnect_msg = making_disconnect_packet(netid_);
        MessagePtr msgptr(new std::string(disconnect_msg));
        send_msg(msgptr);
        ikcp_release(p_kcp_);
        p_kcp_ = NULL;
        call_msg_callback(MsgType::MT_DISCONNECTED, 0, std::make_shared<std::string>());
    }

}