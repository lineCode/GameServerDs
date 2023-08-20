#include "GameEvent.h"

namespace MiniProject
{
    GameEvent::GameEvent(GameEventType _type) : type_(_type)
    {
    }

    void GameEvent::setfrom(uint32_t _from)
    {
        from = _from;
    }
    void GameEvent::setto(uint32_t _to)
    {
        to = _to;
    }

    void GameEvent::setstart(uint64_t _start)
    {
        start = _start;
    }

    void GameEvent::setend(uint64_t _end)
    {
        end = _end;
    }

}