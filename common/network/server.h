#ifndef MINIPROJECT_Server_HPP
#define MINIPROJECT_Server_HPP

#include "EventCallBack.h"
#include "SessionManager.h"
#include <boost/asio.hpp>
#include <list>
#include <memory>
#include <string>

namespace MiniProject
{
    using namespace boost::asio::ip;

    enum SERVICE_TYPE
    {
        TCP_SERVICE = 0,
        UDP_SERVICE = 1,
        BOTH_SERVICE = 2
    };

    class Server
    {
    private:
        /* data */
    public:
        Server(){};
        ~Server(){};

        virtual void load(const std::string &path);

        void init();

        void run(SERVICE_TYPE _type = SERVICE_TYPE::TCP_SERVICE);

        void stop();

        void force_disconnect(const NetId &_netid);

        uint32_t addr();

        uint16_t port();

        uint32_t GetUid();

        bool send_msg_by_uid(uint32_t uid, int cmd_id, MessagePtr _ptr);
        bool send_msg_by_uid(uint32_t uid, int cmd_id, MsgPtr _ptr);

        bool send_msg_by_type(const ConnectType &_type, MessagePtr _ptr);
        bool send_msg_by_id(const NetId &_netId, MessagePtr _ptr);
        bool send_message_by_type(const ConnectType &_type, int cmd_id, MessagePtr _ptr);
        bool send_message_by_id(const NetId &_netId, int cmd_id, MessagePtr _ptr);

        bool send_message_by_type(const ConnectType &_type, int cmd_id, MsgPtr _ptr);
        bool send_message_by_id(const NetId &_netId, int cmd_id, MsgPtr _ptr);

        void send_connect_packet(const ConnectType &_type, const uint32_t &_uid, const NetId &_netid);

    protected:
        SessionManager::SessionManagerPtr _managerPtr;

    protected:
        struct HostDataInfo
        {
            enum ConnectedState
            {
                CST_UNCONNECT = 0,
                CST_CONNECTING = 1,
                CST_CONNECTED = 2
            };

            std::string name;
            std::string addr;
            uint32_t port;
            uint32_t uid; // 服务器的唯一编号
            NetId netId = 0;
            ConnectedState is_connected = CST_UNCONNECT; // 0:未连接，1:正在处理,2已连接
            Session::SessionPtr sessionPtr;
        };

        std::list<HostDataInfo> serverInfoList_;
        HostDataInfo hostInfo_;

    private:
        std::shared_ptr<tcp::acceptor> acceptor_;
        std::shared_ptr<udp::socket> udp_socket_; // udp的主监听端口，负责处理连接层的消息
        std::shared_ptr<boost::asio::io_context> io_context_;
        std::shared_ptr<boost::asio::deadline_timer> connectTimerPtr_;
        bool stopped_ = true;

        udp::endpoint udp_remote_endpoint_;
        enum
        {
            udp_packet_max_length = 1080
        }; // (576-8-20 - 8) * 2
        char udp_data_[1024 * 32];

        std::shared_ptr<boost::asio::signal_set> signals_;
        std::mutex wsMutex_;
        std::list<std::weak_ptr<Session>> weakSessionLists;
        std::shared_ptr<boost::asio::deadline_timer> updateTimer_;

    private:
        void do_accept();
        void do_accept_udp();
        void do_connect();
        void update();
        void signalCallBack();
        void addupdatePacket(std::shared_ptr<Session> ptr);

        void msg_handler(MsgType _type, NetId _netID, CmdId _cmdID, MessagePtr _ptr);

        void connect_msg_handler(NetId _netId, MessagePtr _ptr, bool send_back);

        virtual void initcallback() = 0;
        virtual void onConnected(NetId _netID) = 0;
        virtual void onDisConnected(NetId _netID) = 0;
    };

}

#endif