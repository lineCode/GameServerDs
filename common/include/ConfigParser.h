#ifndef MINIPROJECT_CONFIGPARSER_HPP
#define MINIPROJECT_CONFIGPARSER_HPP

#include <string>
#include <boost/noncopyable.hpp>
#include "cJson/cJSON.h"
#include <list>

namespace MiniProject
{
    struct ServerInfo
    {
        std::string addr;
        int port;
        std::string name;
        uint32_t uid;
    };

    class ConfigParser : public boost::noncopyable
    {
    public:
        ConfigParser(const std::string &filepath);

        ~ConfigParser();

        bool readDefaultServer(ServerInfo &_info);
        int readGateServer(std::list<ServerInfo> &infos);
        int readNodeServer(std::list<ServerInfo> &infos);
        int readGameServer(std::list<ServerInfo> &infos);
        int readDataServer(std::list<ServerInfo> &infos);
        int readBattlServer(std::list<ServerInfo> &infos);
        int readRoomServer(std::list<ServerInfo> &infos);

    private:
        const char *c_node_host_name = "DEFAULT";
        const char *c_node_gate_name = "GateServer";
        const char *c_node_node_name = "NodeServer";
        const char *c_node_game_name = "GameServer";
        const char *c_node_db_name = "DataServer";
        const char *c_node_battle_name = "BattleServer";
        const char *c_node_room_name = "RoomServer";

        const char *c_node_addr_name = "addr";
        const char *c_node_port_name = "port";
        const char *c_node_name = "name";
        const char *c_node_uid = "uid";

        cJSON *root = nullptr;

        int readServers(const char *server_name, std::list<ServerInfo> &infos);
    };

}

#endif