#ifndef MP_MSC_HPP
#define MP_MSC_HPP
#include <string>
#include <unordered_map>
#include "../../../common/include/cJson/cJSON.h"

namespace MiniProject
{
    enum MissionType
    {
        SURVIVAL_MISSION = 0, // 单人生存和多人生存任务
        SINGLE_MISSION = 1,
        SPECIAL_MISSION_1 = 2,
        SPECIAL_MISSION_2 = 3,
        SPECIAL_MISSION_3 = 4
    };

    struct MissionConfig
    {
        MissionType _type;
        std::string name;
        uint32_t time;
        uint32_t score;
        uint32_t target;
    };

    class MissionConfigManager
    {
    public:
        static MissionConfigManager *Instance()
        {
            static MissionConfigManager config;
            return &config;
        }

        const char *c_node_team_survival = "团队生存任务";
        const char *c_node_single_survival = "单人生存任务";
        const char *c_Node_special_task_1 = "特殊任务1";
        const char *c_Node_special_task_2 = "特殊任务2";
        const char *c_Node_special_task_3 = "特殊任务3";

        const char *c_node_time = "生存时间";
        const char *c_node_score = "奖励分数";
        const char *c_node_target = "关卡目标";

        MissionConfig GetConfig(const MissionType &_type);

        int GetMissionId(MissionType _type, int playerid);

    private:
        MissionConfigManager();
        MissionConfig readFromNode(cJSON *root, const char *c_node_name);
        std::unordered_map<std::string, MissionConfig> configMaps;
        std::unordered_map<MissionType, std::string> typeMaps;

    private:
        void readfromfile();
    };

}

#endif