#include "ConfigParser.h"
#include <fstream>
#include <spdlog/spdlog.h>
#include <iostream>

namespace MiniProject
{
    /*********************************************************
           Function:	GetNodeValNumber_JSON
           Description:从json节点读取数字
           Input:      p_device：json节点 c_str_node_name：节点名字
           Output:
           Return:     int：读取到的数字
           **********************************************************/
    static int GetNodeValNumber_JSON(const cJSON *p_device, const char *c_str_node_name)
    {
        cJSON *p_node = cJSON_GetObjectItem(const_cast<cJSON *>(p_device), c_str_node_name);
        if (p_node == NULL || p_node->type != cJSON_Number)
        {
            return -1;
        }
        return p_node->valueint;
    }

    /*********************************************************
    Function:	GetNodeValString_JSON
    Description:从json节点读取字符串
    Input:      p_device：json节点 c_str_node_name：节点名字
    Output:
    Return:     string：读取到的字符串
    **********************************************************/
    static std::string GetNodeValString_JSON(const cJSON *p_device, const char *c_str_node_name)
    {
        cJSON *p_node = cJSON_GetObjectItem(const_cast<cJSON *>(p_device), c_str_node_name);
        if (p_node == NULL || p_node->type != cJSON_String)
        {
            return "";
        }
        return std::string(p_node->valuestring);
    }

    ConfigParser::ConfigParser(const std::string &filepath)
        : root(nullptr)
    {
        std::ifstream fin(filepath.c_str());
        if (!fin.is_open())
        {
            spdlog::error("{} config path not exist", filepath);
            return;
        }
        fin.seekg(0, std::ios::end);
        long long ullFileLen = fin.tellg();
        fin.seekg(0, std::ios::beg);

        char c_device[ullFileLen];
        fin.read(c_device, sizeof(c_device));
        fin.close();

        root = cJSON_Parse(c_device);

        if (root == NULL)
        {
            spdlog::error("{} config format error", filepath);
            return;
        }
    }

    ConfigParser::~ConfigParser()
    {
        if (root != nullptr)
        {
            cJSON_free(root);
        }
    }

    int ConfigParser::readServers(const char *server_name, std::list<ServerInfo> &infos)
    {
        cJSON *p_node = cJSON_GetObjectItem(root, server_name);

        if (p_node == nullptr)
        {
            return 0;
        }
        cJSON *p_node_child = p_node->child;

        int i = 0;
        while (p_node_child != nullptr)
        {
            ServerInfo _info;
            _info.name = GetNodeValString_JSON(p_node_child, c_node_name);
            _info.addr = GetNodeValString_JSON(p_node_child, c_node_addr_name);
            _info.port = GetNodeValNumber_JSON(p_node_child, c_node_port_name);

            p_node_child = p_node_child->next;

            if (_info.port < 0 || _info.addr == "" || _info.name == "")
            {
                continue;
            }
            std::cout << _info.name << std::endl
                      << _info.addr << std::endl
                      << _info.port << std::endl;

            ++i;
            infos.push_back(_info);
        }
        return i;
    }

    bool ConfigParser::readDefaultServer(ServerInfo &_info)
    {
        if (root == nullptr)
        {
            return false;
        }
        cJSON *p_node = cJSON_GetObjectItem(root, c_node_host_name);

        if (p_node == nullptr)
        {
            return false;
        }

        _info.addr = GetNodeValString_JSON(p_node, c_node_addr_name);
        _info.name = GetNodeValString_JSON(p_node, c_node_name);
        _info.port = GetNodeValNumber_JSON(p_node, c_node_port_name);
        _info.uid = GetNodeValNumber_JSON(p_node, c_node_uid);

        if (_info.port < 0 || _info.addr == "" || _info.name == "" || _info.uid < 0)
        {
            return false;
        }
        return true;
    }

    int ConfigParser::readGateServer(std::list<ServerInfo> &infos)
    {
        return readServers(c_node_gate_name, infos);
    }

    int ConfigParser::readNodeServer(std::list<ServerInfo> &infos)
    {
        return readServers(c_node_node_name, infos);
    }

    int ConfigParser::readGameServer(std::list<ServerInfo> &infos)
    {
        return readServers(c_node_game_name, infos);
    }

    int ConfigParser::readDataServer(std::list<ServerInfo> &infos)
    {
        return readServers(c_node_db_name, infos);
    }

    int ConfigParser::readBattlServer(std::list<ServerInfo> &infos)
    {
        return readServers(c_node_battle_name, infos);
    }

    int ConfigParser::readRoomServer(std::list<ServerInfo> &infos)
    {
        return readServers(c_node_room_name, infos);
    }
};
