#include "BattleServer.h"

int main()
{
    MiniProject::BattleServer::Instance()->load("BattleServerConfig.json");

    MiniProject::BattleServer::Instance()->init();

    MiniProject::BattleServer::Instance()->run();

    return 0;
}