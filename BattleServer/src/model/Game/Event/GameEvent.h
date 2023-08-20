#ifndef MINIPROJECT_GAME_EVENT_HPP
#define MINIPROJECT_GAME_EVENT_HPP

#include <string>

namespace MiniProject
{
    enum GameEventType
    {
        GAMEEVENT_UNKNOW = 0,
        GAMEEVENT_KILL = 1,
        GAMEEVENT_CROSSBULEFIRE = 2,
        GAMEVENT_BEARSUSTAIN = 3,
        GAMEVENT_HITBOX = 4,
        GAMEVENT_DISCOVER = 5
    };

    class GameEvent
    {
    public:
        GameEvent(GameEventType _type);
        ~GameEvent() {}

        void setfrom(uint32_t _from);
        void setto(uint32_t _to);
        void setstart(uint64_t _start);
        void setend(uint64_t _end);

    public:
        GameEventType type_;
        uint32_t from;
        uint32_t to;
        uint64_t start;
        uint64_t end;
    };

}

#endif