#include "GameConfig.hpp"
#include <string>
#include "../../../../common/include/cJson/cJSON.h"
#include <spdlog/spdlog.h>
#include <fstream>

namespace MiniProject
{
    uint32_t GameConfig::MISSION_REFRESH_TIME = 1;
    uint32_t GameConfig::SCORE_INCRESE_TIME = 0;   // 每秒增加的分数
    uint32_t GameConfig::SCORE_PLAYER_DEAD = 300;  // 玩家死亡扣分
    uint32_t GameConfig::SCORE_PUBLISH_TIME = 5;   // 分数推送时间
    uint32_t GameConfig::SCORE_PLAYER_HITBOX = 10; // 玩家击碎箱子加分

    char *GameConfig::c_node_refresh_time = "任务刷新时间";
    char *GameConfig::c_node_score_increse = "每秒增加分数";
    char *GameConfig::c_node_playeer_dead = "玩家死亡扣分";
    char *GameConfig::c_node_score_publish_time = "推送时间";

    std::vector<uint32_t> GameConfig::SCORE_LEVEL_INIT =
        {1000, 1000, 1000};

    static int GetNodeValNumber_JSON(const cJSON *p_device, const char *c_str_node_name)
    {
        cJSON *p_node = cJSON_GetObjectItem(const_cast<cJSON *>(p_device), c_str_node_name);
        if (p_node == NULL || p_node->type != cJSON_Number)
        {
            return -1;
        }
        return p_node->valueint;
    }

    void GameConfig::readfromfile()
    {
        std::string filepath = "data/GameConfig.json";

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

        MISSION_REFRESH_TIME = GetNodeValNumber_JSON(root, c_node_refresh_time);
        SCORE_INCRESE_TIME = GetNodeValNumber_JSON(root, c_node_score_increse);
        SCORE_PLAYER_DEAD = GetNodeValNumber_JSON(root, c_node_playeer_dead);
        SCORE_PUBLISH_TIME = GetNodeValNumber_JSON(root, c_node_score_publish_time);
    }
}