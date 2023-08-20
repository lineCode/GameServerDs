#include "TcpSession.h"
#include "spdlog/spdlog.h"

namespace MiniProject
{

    TcpSession::TcpSession(NetId netid, tcp::socket &_sock, SessionCallBack _cb)
        : Session(netid, SessionType::TCP_SESSION, _cb), sock_(std::make_shared<tcp::socket>(std::move(_sock))), stop_(true), m_read_buffer_(new TcpBuffer(1024))
    {
    }

    TcpSession::TcpSession(NetId netid, tcp::socket &_sock, SessionCallBack _cb, ConnectType _type) : Session(netid, SessionType::TCP_SESSION, _cb, _type), sock_(std::make_shared<tcp::socket>(std::move(_sock))), stop_(true), m_read_buffer_(new TcpBuffer(1024))
    {
    }

    TcpSession::~TcpSession()
    {
        if (!stop_)
        {
            stop();
        }
    }

    void TcpSession::start()
    {
        stop_ = false;
        m_st_msg_parse_ = MSG_START;
        recv();
    }

    void TcpSession::recv()
    {
        if (stop_)
        {
            return;
        }
        // char buf[2048];
        buf.resize(2048);
        // bzero(buf, sizeof(buf));
        m_read_buffer_->adjustBuffer();
        sock_->async_read_some(
            boost::asio::buffer(&buf[0], buf.size()),
            [this](boost::system::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    std::cout << "get length:" << length << std::endl;
                    m_read_buffer_->writeToBuffer(&buf[0], length);
                    MsgParseState new_state = MsgParseState::MSG_NONE;
                    int repeat = 3;
                    do
                    {
                        if (new_state != MsgParseState::MSG_NONE)
                        {
                            m_st_msg_parse_ = new_state;
                        }

                        new_state = codec.decodedata(m_read_buffer_.get(), m_st_msg_parse_);

                        if (new_state == MsgParseState::MSG_NONE)
                        {
                            --repeat;
                        }
                        else if (new_state == MsgParseState::MSG_START &&
                                 m_st_msg_parse_ == MsgParseState::MSG_END)
                        {
                            CmdId cmd_id = codec.cmd_id();
                            std::string data = std::move(codec.data());
                            call_msg_callback(MsgType::MT_RECVMSG, cmd_id, std::make_shared<std::string>(data));
                            std::cout << cmd_id << std::endl;
                        }
                    } while (new_state != m_st_msg_parse_ && repeat > 0);
                    if (repeat == 0)
                    {
                        // 解析出错
                        stop();
                    }
                    else
                    {
                        recv();
                    }
                }
                else
                {
                    if (!(ec == boost::asio::error::interrupted ||
                          ec == boost::asio::error::would_block))
                    {
                        if (!stop_)
                        {
                            stop();
                        }
                    }
                }
            });
    }

    void TcpSession::stop()
    {
        if (stop_)
        {
            return;
        }
        stop_ = true;
        sock_->cancel(); // 取消异步事件
        sock_->close();  // 关闭socket连接
        call_msg_callback(MsgType::MT_DISCONNECTED, 0, std::make_shared<std::string>());
    }

    void TcpSession::send_msg(MessagePtr ptr)
    {
        if (!stop_)
        {
            std::string _data = *(ptr.get());
            // sock_->send(boost::asio::buffer(_data), _data.size());
            sock_->write_some(boost::asio::buffer(_data));
        }
    }

}