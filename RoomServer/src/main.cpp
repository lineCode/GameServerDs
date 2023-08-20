#include "RoomServer.h"

int main()
{
    MiniProject::RoomServer::Instance()->load("RoomServerConfig.json");

    MiniProject::RoomServer::Instance()->init();

    MiniProject::RoomServer::Instance()->run();

    return 0;
}