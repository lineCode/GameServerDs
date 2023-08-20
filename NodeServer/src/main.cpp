#include "NodeServer.h"

int main()
{
    MiniProject::NodeServer::Instance()->load("NodeServerConfig.json");

    MiniProject::NodeServer::Instance()->init();

    MiniProject::NodeServer::Instance()->run();

    return 0;
}