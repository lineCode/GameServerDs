#include "MissionConfig.h"
#include <spdlog/spdlog.h>
#include <fstream>
#include <iostream>

namespace MiniProject
{
    static int GetNodeValNumber_JSON(const cJSON *p_device, const char *c_str_node_name)
    {
        cJSON *p_node = cJSON_GetObjectItem(const_cast<cJSON *>(p_device), c_str_node_name);
        if (p_node == NULL || p_node->type != cJSON_Number)
        {
            return -1;
        }
        return p_node->valueint;
    }

    static std::string GetNodeValString_JSON(const cJSON *p_device, const char *c_str_node_name)
    {
        cJSON *p_node = cJSON_GetObjectItem(const_cast<cJSON *>(p_device), c_str_node_name);
        if (p_node == NULL || p_node->type != cJSON_String)
        {
            return "";
        }
        return std::string(p_node->valuestring);
    }

    MissionConfig MissionConfigManager::readFromNode(cJSON *root, const char *c_node_name)
    {

        MissionConfig config;
        cJSON *p_node = cJSON_GetObjectItem(root, c_node_name);
        if (p_node == nullptr)
        {
            return config;
        }

        config.name = c_node_name;
        config.score = 0;
        config.time = 0;
        config.target = 0;

        int ret = GetNodeValNumber_JSON(p_node, c_node_time);
        if (ret != -1)
        {
            config.time = ret;
        }

        if (c_node_name == c_Node_special_task_2)
        {
            config.target = GetNodeValNumber_JSON(p_node, c_node_target);
            if (ret != -1)
            {
                config.target = ret;
            }
        }

        ret = GetNodeValNumber_JSON(p_node, c_node_score);
        if (ret != -1)
        {
            config.score = ret;
        }
        return config;
    }

    MissionConfigManager::MissionConfigManager()
    {
        typeMaps[SURVIVAL_MISSION] = c_node_team_survival;
        typeMaps[SINGLE_MISSION] = c_node_single_survival;
        typeMaps[SPECIAL_MISSION_1] = c_Node_special_task_1;
        typeMaps[SPECIAL_MISSION_2] = c_Node_special_task_2;
        typeMaps[SPECIAL_MISSION_3] = c_Node_special_task_3;
        readfromfile();
    }
    void MissionConfigManager::readfromfile()
    {
        std::string filepath = "data/Mission.json";

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

        cJSON *root = cJSON_Parse(c_device);

        if (root == NULL)
        {
            spdlog::error("{} config format error", filepath);
            return;
        }

        configMaps[c_node_team_survival] = readFromNode(root, c_node_team_survival);
        configMaps[c_node_single_survival] = readFromNode(root, c_node_single_survival);
        configMaps[c_Node_special_task_1] = readFromNode(root, c_Node_special_task_1);
        configMaps[c_Node_special_task_2] = readFromNode(root, c_Node_special_task_2);
        configMaps[c_Node_special_task_3] = readFromNode(root, c_Node_special_task_3);
    }

    MissionConfig MissionConfigManager::GetConfig(const MissionType &_type)
    {
        return configMaps[typeMaps[_type]];
    }

    int MissionConfigManager::GetMissionId(MissionType _type, int playerid)
    {
        if (_type == MissionType::SURVIVAL_MISSION)
        {
            return 1;
        }
        else if (_type == MissionType::SINGLE_MISSION)
        {
            return playerid + 1;
        }
        else if (_type == MissionType::SPECIAL_MISSION_1)
        {
            return playerid + 5;
        }
        else if (_type == MissionType::SPECIAL_MISSION_2)
        {
            return playerid + 9;
        }
        else if (_type == MissionType::SPECIAL_MISSION_3)
        {
            return playerid + 13;
        }
        return _type;
    }

}