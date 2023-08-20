#ifndef MINIPROJECT_NETWORK_TYPE_HPP
#define MINIPROJECT_NETWORK_TYPE_HPP

#include <boost/asio.hpp>
#include <functional>
#include <memory>
#include <string>

namespace MiniProject
{
    using namespace boost::asio::ip;

    // 连接的服务器的标识
    enum ConnectType
    {
        CT_DEFAULT = 0,
        CT_GATE_SERVER = 1,
        CT_LOGIN_SERVER = 2,
        CT_GAME_SERVER = 3,
        CT_NODE_SERVER = 4,
        CT_ROOM_SERVER = 5,
        CT_DB_SERVER = 6,
        CT_BATTLE_SERVER = 7
    };

    static const std::string LOGIN_SERVER_NAME = "LoginServer";
    static const std::string GATE_SERVER_NAME = "GateServer";
    static const std::string NODE_SERVER_NAME = "NodeServer";
    static const std::string GAME_SERVER_NAME = "GameServer";
    static const std::string DB_SERVER_NAME = "DataServer";
    static const std::string BATTLE_SERVER_NAME = "BattleServer";
    static const std::string ROOM_SERVER_NAME = "RoomServer";

    enum NetWorkType
    {
        NT_DEFAULT = 0,
        NT_TCP = 1,
        NT_UDP = 2
    };

    ConnectType GetConnectType(const std::string &name);
}

#endif