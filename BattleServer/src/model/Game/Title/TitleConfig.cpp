#include "TitleConfig.h"
#include "../../../../../common/include/cJson/cJSON.h"
#include <spdlog/spdlog.h>
#include <fstream>
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

    static Title readFromNode(cJSON *root, const char *c_node_name)
    {
        Title config;
        cJSON *p_node = cJSON_GetObjectItem(root, c_node_name);
        if (p_node == nullptr)
        {
            return config;
        }

        config.name = c_node_name;
        config.titleid = GetNodeValNumber_JSON(p_node, TitleConfigName().c_node_title.c_str());
        config.score = GetNodeValNumber_JSON(p_node, TitleConfigName().c_node_score.c_str());
        config.priority = GetNodeValNumber_JSON(p_node, TitleConfigName().c_node_priority.c_str());

        return config;
    }

    TitleConfig::TitleConfig()
    {
        readfromfile();
    }

    void TitleConfig::readfromfile()
    {
        std::string filepath = "data/Title.json";

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

        configMaps[TitleConfigName().one_life] = readFromNode(root, TitleConfigName().one_life.c_str());
        idmaps[configMaps[TitleConfigName().one_life].titleid] = TitleConfigName().one_life;

        configMaps[TitleConfigName().longestexistence] = readFromNode(root, TitleConfigName().longestexistence.c_str());
        idmaps[configMaps[TitleConfigName().longestexistence].titleid] = TitleConfigName().longestexistence;

        configMaps[TitleConfigName().killmost] = readFromNode(root, TitleConfigName().killmost.c_str());
        idmaps[configMaps[TitleConfigName().killmost].titleid] = TitleConfigName().killmost;

        configMaps[TitleConfigName().nokill] = readFromNode(root, TitleConfigName().nokill.c_str());
        idmaps[configMaps[TitleConfigName().nokill].titleid] = TitleConfigName().nokill;

        configMaps[TitleConfigName().mostbeared] = readFromNode(root, TitleConfigName().mostbeared.c_str());
        idmaps[configMaps[TitleConfigName().mostbeared].titleid] = TitleConfigName().mostbeared;

        configMaps[TitleConfigName().nobear] = readFromNode(root, TitleConfigName().nobear.c_str());
        idmaps[configMaps[TitleConfigName().nobear].titleid] = TitleConfigName().nobear;

        configMaps[TitleConfigName().mostdeath] = readFromNode(root, TitleConfigName().mostdeath.c_str());
        idmaps[configMaps[TitleConfigName().mostdeath].titleid] = TitleConfigName().mostdeath;

        configMaps[TitleConfigName().traveler] = readFromNode(root, TitleConfigName().traveler.c_str());
        idmaps[configMaps[TitleConfigName().traveler].titleid] = TitleConfigName().traveler;
    }

    Title TitleConfig::find(const std::string &name)
    {
        return configMaps[name];
    }

}