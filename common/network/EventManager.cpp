#include "EventManager.h"

namespace MiniProject
{
    void EventManager::RegisterEventHandler(EventId _eid, EventFunc _func)
    {
        m_map_id_funcs_[_eid] = _func;
    }

    void EventManager::UnRegisterEventHandler(EventId _eid, EventFunc _func)
    {
        auto it = m_map_id_funcs_.find(_eid);

        if (it != m_map_id_funcs_.end())
        {
            m_map_id_funcs_.erase(it);
        }
    }

    bool EventManager::findEventHandlerById(EventId _eid, EventFunc &_func)
    {
        auto it = m_map_id_funcs_.find(_eid);

        if (it != m_map_id_funcs_.end())
        {
            _func = it->second;
            return true;
        }

        return false;
    }

}