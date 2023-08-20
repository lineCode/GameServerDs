#ifndef MP_TC_HPP
#define MP_TC_HPP
#include <string>
#include <unordered_map>

namespace MiniProject
{
    struct Title
    {
        uint32_t titleid;
        uint32_t score;
        uint32_t priority;
        std::string name;

        bool operator>(const Title &rhs)
        {
            if (priority != rhs.priority)
            {
                return priority > rhs.priority;
            }
            return score >= rhs.score;
        }

        bool operator<(const Title &rhs)
        {
            if (priority != rhs.priority)
            {
                return priority < rhs.priority;
            }
            return score < rhs.score;
        }

        friend bool operator<(const Title &lhs, const Title &rhs)
        {
            if(lhs.priority != rhs.priority)
            {
                return lhs.priority < rhs.priority;
            }
            return lhs.score < rhs.score;
        }
    };

    class TitleConfigName
    {
    public:
        const std::string one_life = "一命通关";
        const std::string longestexistence = "最长存活时间";
        const std::string killmost = "杀死队友最多的玩家";
        const std::string nokill = "大慈大悲";
        const std::string mostbeared = "忍者神喵";
        const std::string nobear = "无须再忍";
        const std::string mostdeath = "亡者归来";
        const std::string traveler = "旅行猫";

        const std::string c_node_title = "titleid";
        const std::string c_node_score = "score";
        const std::string c_node_priority = "priority";
    };

    class TitleConfig
    {
    public:
        static TitleConfig *Instance()
        {
            static TitleConfig config;
            return &config;
        }

        Title find(const std::string &name);

    private:
        TitleConfig();
        void readfromfile();

    private:
        std::unordered_map<std::string, Title> configMaps;
        std::unordered_map<uint32_t, std::string> idmaps;
    };

}

#endif