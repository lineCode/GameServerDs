#ifndef MINPROJECT_GAMECONFIG_HPP
#define MINPROJECT_GAMECONFIG_HPP

#include <string>
#include <vector>

namespace MiniProject
{
    class GameConfig
    {
    public:
        static uint32_t MISSION_REFRESH_TIME;
        static uint32_t SCORE_INCRESE_TIME;  // 每秒增加的分数
        static uint32_t SCORE_PLAYER_DEAD;   // 玩家死亡扣分
        static uint32_t SCORE_PUBLISH_TIME;  // 分数推送时间
        static uint32_t SCORE_PLAYER_HITBOX; // 击碎箱子加分
        static std::vector<uint32_t> SCORE_LEVEL_INIT;//关卡初始分数

        static char *c_node_refresh_time;
        static char *c_node_score_increse;
        static char *c_node_playeer_dead;
        static char *c_node_score_publish_time;

        static void readfromfile();
    };

} // namespace MiniProject

#endif