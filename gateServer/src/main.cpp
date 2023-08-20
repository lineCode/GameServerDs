#include "GateServer.h"

int main()
{
    MiniProject::GateServer::Instance()->load("GateServerConfig.json");

    MiniProject::GateServer::Instance()->init();

    MiniProject::GateServer::Instance()->run(MiniProject::SERVICE_TYPE::BOTH_SERVICE);

    return 0;
}