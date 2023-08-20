#include "DBServer.h"
#include "../../../common/datahandle/mysqlPool.h"

int main()
{

    MiniProject::connection_pool::GetInstance()->init("127.0.0.1", "root", "123456", "MiniProject", 3306, 8);
    
    MiniProject::DBServer::Instance()->load("DataServerConfig.json");

    MiniProject::DBServer::Instance()->init();

    MiniProject::DBServer::Instance()->run();

    return 0;
}