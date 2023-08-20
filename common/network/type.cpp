#include "type.h"

namespace MiniProject
{

    ConnectType GetConnectType(const std::string &name)
    {
        if (name.length() >= LOGIN_SERVER_NAME.length() && name.substr(0, LOGIN_SERVER_NAME.length()) == LOGIN_SERVER_NAME)
        {
            return CT_LOGIN_SERVER;
        }
        else if (name.length() >= GATE_SERVER_NAME.length() && name.substr(0, GATE_SERVER_NAME.length()) == GATE_SERVER_NAME)
        {
            return CT_GATE_SERVER;
        }
        else if (name.length() >= NODE_SERVER_NAME.length() && name.substr(0, NODE_SERVER_NAME.length()) == NODE_SERVER_NAME)
        {
            return CT_NODE_SERVER;
        }
        else if (name.length() >= GAME_SERVER_NAME.length() && name.substr(0, GAME_SERVER_NAME.length()) == GAME_SERVER_NAME)
        {
            return CT_GAME_SERVER;
        }
        else if (name.length() >= DB_SERVER_NAME.length() && name.substr(0, DB_SERVER_NAME.length()) == DB_SERVER_NAME)
        {
            return CT_DB_SERVER;
        }
        else if (name.length() >= BATTLE_SERVER_NAME.length() && name.substr(0, BATTLE_SERVER_NAME.length()) == BATTLE_SERVER_NAME)
        {
            return CT_BATTLE_SERVER;
        }
        else if (name.length() >= ROOM_SERVER_NAME.length() && name.substr(0, ROOM_SERVER_NAME.length()) == ROOM_SERVER_NAME)
        {
            return CT_ROOM_SERVER;
        }
        return CT_DEFAULT;
    }

}