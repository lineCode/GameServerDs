#ifndef MINIPROJECT_EVENTMANAGER_HPP
#define MINIPROJECT_EVENTMANAGER_HPP

#include "EventCallBack.h"
#include <boost/noncopyable.hpp>
#include <unordered_map>

namespace MiniProject
{

    class EventManager : private boost::noncopyable
    {
    public:
        static EventManager *Instance()
        {
            static EventManager _manager;
            return &_manager;
        }

        void RegisterEventHandler(EventId _eid, EventFunc _func);
        void UnRegisterEventHandler(EventId _eid, EventFunc _func);
        bool findEventHandlerById(EventId _eid, EventFunc &_func);

    private:
        std::unordered_map<EventId, EventFunc> m_map_id_funcs_;
    };

} // namespace MiniProject

#endif