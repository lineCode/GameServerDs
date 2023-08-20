#include "LoginServer.h"

int main()
{
    MiniProject::LoginServer::Instance()->load("LoginServerConfig.json");

    MiniProject::LoginServer::Instance()->init();

    MiniProject::LoginServer::Instance()->run();

    return 0;
}