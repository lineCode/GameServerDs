#include "GameServer.h"
#include "../../../common/datahandle/redisPool.h"


int main()
{
    using namespace MiniProject;

    MiniProject::RedisConnectionPool::GetInstance()->init("127.0.0.1", 6379, "123456", 8);
    // MiniProject::connection_pool::GetInstance()->init("127.0.0.1", "root", "Root123456~", "MiniProject", 3306, 8);

    GameServer::Instance()->load("GameServerConfig.json");

    GameServer::Instance()->init();

    GameServer::Instance()->run();

    return 0;
}