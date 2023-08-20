#include "server.h"
#include "../include/ConfigParser.h"
#include "../include/clock.h"
#include "../include/codec.h"
#include "../include/ikcp.h"
#include "../protobuf/server_connetion.pb.h"
#include "EventManager.h"
#include "KcpConectionPacket.h"
#include "KcpSession.h"
#include "TcpSession.h"
#include "disPatcher.h"
#include "spdlog/spdlog.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <unistd.h>

namespace MiniProject
{

    void Server::load(const std::string &filepath)
    {
        ConfigParser _parse(filepath);

        ServerInfo _hostinfo;
        if (!_parse.readDefaultServer(_hostinfo))
        {
            spdlog::error("Parse config error,exit");
            exit(0);
        }
        hostInfo_.name = _hostinfo.name;
        hostInfo_.addr = _hostinfo.addr;
        hostInfo_.port = _hostinfo.port;
        hostInfo_.uid = _hostinfo.uid;

        std::list<ServerInfo> _infos;
        if (_parse.readGateServer(_infos) > 0)
        {
            for (ServerInfo &_info : _infos)
            {
                HostDataInfo _hostData;
                _hostData.name = _info.name;
                _hostData.addr = _info.addr;
                _hostData.port = _info.port;
                serverInfoList_.push_back(_hostData);
            }
            std::list<ServerInfo>().swap(_infos);
        }

        if (_parse.readNodeServer(_infos) > 0)
        {
            for (ServerInfo &_info : _infos)
            {
                HostDataInfo _hostData;
                _hostData.name = _info.name;
                _hostData.addr = _info.addr;
                _hostData.port = _info.port;
                serverInfoList_.push_back(_hostData);
            }
            std::list<ServerInfo>().swap(_infos);
        }

        if (_parse.readGameServer(_infos) > 0)
        {
            for (ServerInfo &_info : _infos)
            {
                HostDataInfo _hostData;
                _hostData.name = _info.name;
                _hostData.addr = _info.addr;
                _hostData.port = _info.port;
                serverInfoList_.push_back(_hostData);
            }
            std::list<ServerInfo>().swap(_infos);
        }

        if (_parse.readDataServer(_infos) > 0)
        {
            for (ServerInfo &_info : _infos)
            {
                HostDataInfo _hostData;
                _hostData.name = _info.name;
                _hostData.addr = _info.addr;
                _hostData.port = _info.port;
                serverInfoList_.push_back(_hostData);
            }
            std::list<ServerInfo>().swap(_infos);
        }

        if (_parse.readBattlServer(_infos) > 0)
        {
            for (ServerInfo &_info : _infos)
            {
                HostDataInfo _hostData;
                _hostData.name = _info.name;
                _hostData.addr = _info.addr;
                _hostData.port = _info.port;
                serverInfoList_.push_back(_hostData);
            }
            std::list<ServerInfo>().swap(_infos);
        }

        if (_parse.readRoomServer(_infos) > 0)
        {
            for (ServerInfo &_info : _infos)
            {
                HostDataInfo _hostData;
                _hostData.name = _info.name;
                _hostData.addr = _info.addr;
                _hostData.port = _info.port;
                serverInfoList_.push_back(_hostData);
            }
            std::list<ServerInfo>().swap(_infos);
        }
    }

    void Server::init()
    {
        io_context_.reset(new boost::asio::io_context());

        if (!io_context_)
        {
            spdlog::error("IO CONTEXT init failed,exit");
            exit(1);
        }

        // signals_.reset(new boost::asio::signal_set(*io_context_.get()));
        // signals_->add(SIGHUP);
        // signals_->add(SIGINT);
        // signalCallBack();

        connectTimerPtr_.reset(new boost::asio::deadline_timer(*io_context_.get()));

        _managerPtr.reset(new SessionManager());

        initcallback();

        EventManager::Instance()->RegisterEventHandler(pb::SS_CONNECT_REQ_ID, std::bind(&Server::connect_msg_handler, this, std::placeholders::_1, std::placeholders::_2, true));
        EventManager::Instance()->RegisterEventHandler(pb::SS_CONNECT_RSP_ID, std::bind(&Server::connect_msg_handler, this, std::placeholders::_1, std::placeholders::_2, false));
    }

    void Server::run(SERVICE_TYPE _type)
    {
        stopped_ = false;
        if (_type == SERVICE_TYPE::TCP_SERVICE || _type == SERVICE_TYPE::BOTH_SERVICE)
        {
            do_accept();
        }

        if (_type == SERVICE_TYPE::UDP_SERVICE || _type == SERVICE_TYPE::BOTH_SERVICE)
        {
            do_accept_udp();
            updateTimer_.reset(new boost::asio::deadline_timer(*io_context_.get()));
            update();
        }
        do_connect();
        io_context_->run();
    }

    void Server::update()
    {
        if (stopped_)
        {
            return;
        }
        uint32_t cur_clock_ = iclock();

        {
            std::lock_guard<std::mutex> lock(wsMutex_);
            for (auto it = weakSessionLists.begin(); it != weakSessionLists.end();)
            {
                Session::SessionPtr sptr = (*it).lock();
                if (!sptr)
                {
                    it = weakSessionLists.erase(it);
                    continue;
                }
                sptr->update(cur_clock_);
                ++it;
            }
        }
        updateTimer_->expires_from_now(boost::posix_time::millisec(10));
        updateTimer_->async_wait(std::bind(&Server::update, this));
    }

    void Server::signalCallBack()
    {
        signals_->async_wait([this](const boost::system::error_code &error, int signal_number)
                             {
            if (!error)
            {
                // 处理信号
                if (signal_number == SIGHUP)
                {
                    std::cout << "Received SIGHUP signal." << std::endl;
                    // 执行相应的操作
                    // ...
                }
                else if(signal_number == SIGINT)
                {
                    std::cout << "Received SIGINT signal." << std::endl;
                }
                signalCallBack();
            } });
    }

    void Server::force_disconnect(const NetId &netId)
    {
        Session::SessionPtr _ptr = _managerPtr->findSessionByNetId(netId);
        if (_ptr)
        {
            _ptr->stop();
        }
    }

    void Server::do_accept()
    {
        if (stopped_)
        {
            return;
        }

        if (!acceptor_)
        {
            acceptor_.reset(new tcp::acceptor(*io_context_.get(), tcp::endpoint(boost::asio::ip::address_v4::from_string(hostInfo_.addr), hostInfo_.port)));

            if (!acceptor_)
            {
                spdlog::error("acceptor init error,exit");
                exit(1);
            }
        }

        acceptor_->async_accept(
            [this](boost::system::error_code ec, tcp::socket socket)
            {
                if (!ec)
                {
                    NetId _id = _managerPtr->GetNewTcpId();
                    TcpSession *session = new TcpSession(_id, socket, std::bind(&Server::msg_handler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
                    Session::SessionPtr _ptr(session);

                    session->start();

                    _managerPtr->addSessionById(_ptr->GetNetId(), _ptr);

                    msg_handler(MsgType::MT_CONNECTED, _id, 0, MessagePtr());
                }
                else
                {
                    if (ec == boost::asio::error::operation_aborted ||
                        ec == boost::asio::error::eof)
                    {
                        spdlog::info("Server Stop accept");
                        stopped_ = true;
                        stop();
                    }
                    else if (ec == boost::asio::error::interrupted ||
                             ec == boost::asio::error::would_block)
                    {
                        do_accept();
                    }
                }
                do_accept();
            });
    }

    void Server::addupdatePacket(std::shared_ptr<Session> ptr)
    {
        std::lock_guard<std::mutex> lock(wsMutex_);
        weakSessionLists.push_back(ptr);
    }

    void Server::do_accept_udp()
    {
        // 接收udp的连接
        if (stopped_)
        {
            return;
        }

        spdlog::info("void Server::do_accept_udp()");
        if (!udp_socket_)
        {
            udp_socket_.reset(new udp::socket(*io_context_.get(), udp::endpoint(boost::asio::ip::address::from_string(hostInfo_.addr), hostInfo_.port)));
            if (!udp_socket_)
            {
                spdlog::error("acceptor init error,exit");
                exit(1);
            }
        }

        udp_socket_->async_receive_from(
            boost::asio::buffer(udp_data_, sizeof(udp_data_)), udp_remote_endpoint_,
            [this](const boost::system::error_code &error, size_t bytes_recvd)
            {
                if (!error && bytes_recvd > 0)
                {
                    if (is_connect_packet(udp_data_, bytes_recvd))
                    {
                        {
                            NetId conv = _managerPtr->GetNewKcpConv();
                            spdlog::info("Get a NetConnection {}", conv);
                            std::string send_back_msg = making_send_back_conv_packet(conv);
                            udp_socket_->send_to(boost::asio::buffer(send_back_msg), udp_remote_endpoint_);
                            KcpSession *session = new KcpSession(conv, udp_socket_, std::bind(&Server::msg_handler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
                            session->set_udp_remote_endpoint(udp_remote_endpoint_);
                            Session::SessionPtr ptr(static_cast<Session *>(session));
                            addupdatePacket(ptr);
                            _managerPtr->addSessionById(conv, ptr);
                            ptr->start();
                            // 注册连接需要处理
                            msg_handler(MsgType::MT_CONNECTED, conv, 0, MessagePtr());
                        }
                    }
                    else if (is_disconnect_packet(udp_data_, bytes_recvd))
                    {
                        NetId _netId = grab_conv_from_disconnect_packet(udp_data_, bytes_recvd);
                        spdlog::info("Handle DisConnect from Netid {}", _netId);
                        Session::SessionPtr ptr = _managerPtr->findSessionByNetId(_netId);
                        if (ptr)
                        {
                            ptr->stop();
                        }
                        // msg_handler(MsgType::MT_DISCONNECTED, _netId, 0, MessagePtr());
                    }
                    else
                    {
                        NetId conv;
                        int ret = ikcp_get_conv(udp_data_, bytes_recvd, (IUINT32 *)&conv);
                        if (ret != 0)
                        {
                            Session::SessionPtr ptr = _managerPtr->findSessionByNetId(conv);
                            if (ptr)
                            {
                                std::string data(udp_data_, bytes_recvd);
                                ptr->input(data);
                            }
                        }
                    }
                    do_accept_udp();
                }
                else
                {
                    if (error == boost::asio::error::interrupted ||
                        error == boost::asio::error::would_block)
                    {
                        do_accept_udp();
                    }
                    else
                    { // udp接收错误
                        udp_socket_->close();
                        udp_socket_.reset();
                    }
                }
            });
    }

    void Server::do_connect()
    {
        for (HostDataInfo &_host : serverInfoList_)
        {
            if (_host.is_connected == HostDataInfo::ConnectedState::CST_UNCONNECT)
            {
                if (_host.addr == "" || _host.name == "" || _host.port == 0)
                {
                    continue;
                }

                ConnectType _type = GetConnectType(_host.name);
                if (!io_context_)
                {
                    break;
                }
                tcp::socket sock(*io_context_.get());
                NetId _id = _managerPtr->GetNewTcpId();
                TcpSession *session = new TcpSession(_id, sock, std::bind(&Server::msg_handler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4), _type);
                _host.sessionPtr.reset(session);
                _host.netId = _id;
                _host.is_connected = HostDataInfo::ConnectedState::CST_CONNECTING;
                spdlog::info("Try to connect to the server {} {}:{}", _host.name, _host.addr, _host.port);
                session->GetSocketPtr()->async_connect(tcp::endpoint(make_address(_host.addr), _host.port), [this, &_host](const boost::system::error_code &ec)
                                                       {
                                            if (!ec)
                                            {
                                                ConnectType _type = GetConnectType(this->hostInfo_.name);
                                                _host.is_connected = HostDataInfo::ConnectedState::CST_CONNECTED;
                                                _managerPtr->addSessionById(_host.netId,_host.sessionPtr);
                                                _host.sessionPtr->start();
                                                msg_handler(MsgType::MT_CONNECTED, _host.netId, 0, MessagePtr());
                                                send_connect_packet(_type,this->hostInfo_.uid,_host.netId);
                                                _host.sessionPtr.reset();
                                           
                                            }
                                            else
                                            {
                                                _host.is_connected = HostDataInfo::ConnectedState::CST_UNCONNECT;
                                                _host.sessionPtr->stop();
                                                _host.sessionPtr.reset();
                                            } });
            }
        }

        connectTimerPtr_->expires_from_now(boost::posix_time::seconds(3));
        connectTimerPtr_->async_wait([this](const boost::system::error_code &ec)
                                     {
                                        if(!ec)
                                        {
                                            if (!stopped_)
                                            {
                                                do_connect();
                                            } 
                                        } });
    }

    void Server::msg_handler(MsgType _type, NetId _netID, CmdId _cmdID, MessagePtr _ptr)
    {
        if (_type == MsgType::MT_CONNECTED)
        {
            spdlog::debug("Get new Conection from {}", _netID);
            onConnected(_netID);
        }
        else if (_type == MsgType::MT_DISCONNECTED)
        {
            spdlog::debug("DisConneted from {}", _netID);
            onDisConnected(_netID);
            _managerPtr->removeSessionPById(_netID);
            for (HostDataInfo &_host : serverInfoList_)
            {
                if (_host.netId == _netID)
                {
                    _host.netId = 0;
                    _host.is_connected = HostDataInfo::ConnectedState::CST_UNCONNECT;
                    break;
                }
            }
        }
        else if (_type == MsgType::MT_RECVMSG)
        {
            spdlog::debug("RecvMsg from {}", _netID);
            EventFunc _func;
            if (EventManager::Instance()->findEventHandlerById(_cmdID, _func))
            {
                DisPatcher::Instance()->enqueueTask(std::bind(_func, _netID, _ptr), &_netID);
            }
        }
    }

    void Server::stop()
    {
        spdlog::info("Server Stop");
    }

    bool Server::send_msg_by_uid(uint32_t uid, int cmd_id, MessagePtr _ptr)
    {
        Session::SessionPtr _session = _managerPtr->findSessionByUid(uid);

        if (_session)
        {
            int len = 0;
            std::string _data = ProtoCodec().encodebytes(*_ptr.get(), cmd_id, len);
            if (len == 0)
            {
                return false;
            }
            _ptr.reset(new std::string(std::move(_data)));
            _session->send_msg(_ptr);
            return true;
        }
        return false;
    }

    bool Server::send_msg_by_uid(uint32_t uid, int cmd_id, MsgPtr _ptr)
    {
        Session::SessionPtr _session = _managerPtr->findSessionByUid(uid);

        if (_session)
        {
            int len = 0;
            std::string _data = ProtoCodec().encodebytes(_ptr.get(), cmd_id, len);
            if (len == 0)
            {
                return false;
            }
            MessagePtr _msgptr(new std::string(std::move(_data)));
            _session->send_msg(_msgptr);
            return true;
        }
        return false;
    }

    bool Server::send_msg_by_type(const ConnectType &_type, MessagePtr _ptr)
    {
        Session::SessionPtr _session = _managerPtr->findSessionByType(_type);

        if (_session)
        {
            _session->send_msg(_ptr);
            return true;
        }
        return false;
    }

    bool Server::send_msg_by_id(const NetId &_netId, MessagePtr _ptr)
    {
        Session::SessionPtr _session = _managerPtr->findSessionByNetId(_netId);

        if (_session)
        {
            _session->send_msg(_ptr);
            return true;
        }
        return false;
    }

    bool Server::send_message_by_type(const ConnectType &_type, int cmd_id, MessagePtr _ptr)
    {
        Session::SessionPtr _session = _managerPtr->findSessionByType(_type);

        if (_session)
        {
            int len = 0;
            std::string _data = ProtoCodec().encodebytes(*_ptr.get(), cmd_id, len);
            if (len == 0)
            {
                return false;
            }
            _ptr.reset(new std::string(std::move(_data)));
            _session->send_msg(_ptr);
            return true;
        }
        return false;
    }

    bool Server::send_message_by_id(const NetId &_netId, int cmd_id, MessagePtr _ptr)
    {
        Session::SessionPtr _session = _managerPtr->findSessionByNetId(_netId);

        if (_session)
        {
            int len = 0;
            std::string _data = ProtoCodec().encodebytes(*_ptr.get(), cmd_id, len);
            if (len == 0)
            {
                return false;
            }
            _ptr.reset(new std::string(std::move(_data)));
            _session->send_msg(_ptr);
            return true;
        }
        return false;
    }

    bool Server::send_message_by_type(const ConnectType &_type, int cmd_id, MsgPtr _ptr)
    {
        Session::SessionPtr _session = _managerPtr->findSessionByType(_type);

        if (_session)
        {
            int len = 0;
            std::string _data = ProtoCodec().encodebytes(_ptr.get(), cmd_id, len);
            if (len == 0)
            {
                return false;
            }
            MessagePtr _msgptr(new std::string(std::move(_data)));
            _session->send_msg(_msgptr);
            return true;
        }

        return false;
    }

    bool Server::send_message_by_id(const NetId &_netId, int cmd_id, MsgPtr _ptr)
    {
        Session::SessionPtr _session = _managerPtr->findSessionByNetId(_netId);

        if (_session)
        {
            int len = 0;
            std::string _data = ProtoCodec().encodebytes(_ptr.get(), cmd_id, len);
            if (len == 0)
            {
                return false;
            }
            MessagePtr _msgptr(new std::string(std::move(_data)));
            _session->send_msg(_msgptr);
            return true;
        }

        return false;
    }

    void Server::send_connect_packet(const ConnectType &_type, const uint32_t &_uid, const NetId &_netid)
    {
        Session::SessionPtr _ptr = _managerPtr->findSessionByNetId(_netid);
        if (_ptr)
        {
            pb::SSConnectPacket req;
            req.set_sstype((int)_type);
            req.set_serveruid(_uid);
            int len;
            std::string _data = ProtoCodec().encodebytes(&req, pb::SS_CONNECT_REQ_ID, len);
            MessagePtr _msgptr(new std::string(std::move(_data)));
            _ptr->send_msg(_msgptr);
        }
    }

    void Server::connect_msg_handler(NetId _netId, MessagePtr _ptr, bool send_back)
    {
        spdlog::info("connect_msg_handler {}", _netId);
        pb::SSConnectPacket req;
        if (req.ParseFromString(*_ptr.get()))
        {
            ConnectType _type = (ConnectType)req.sstype();
            uint32_t _uid = req.serveruid();
            _managerPtr->updateSessionType(_netId, _type);
            _managerPtr->updateSessionUiD(_netId, _uid);
            if (send_back)
            {
                ConnectType _type = GetConnectType(hostInfo_.name);
                pb::SSConnectPacket req;
                req.set_sstype((int)_type);
                req.set_serveruid(hostInfo_.uid);
                int len;
                std::string _data = ProtoCodec().encodebytes(&req, pb::SS_CONNECT_RSP_ID, len);
                MessagePtr _msgptr(new std::string(std::move(_data)));
                send_message_by_id(_netId, pb::SS_CONNECT_RSP_ID, _msgptr);
            }
        }
    }

    uint32_t Server::addr()
    {
        return inet_addr(hostInfo_.addr.c_str());
    }

    uint16_t Server::port()
    {
        return (uint16_t)hostInfo_.port;
    }

    uint32_t Server::GetUid()
    {
        return hostInfo_.uid;
    }

}