#include "Mission.h"
#include "MissionConfig.h"

namespace MiniProject
{
    Mission::Mission(MissionType _type)
    {
        MissionConfig config = MissionConfigManager::Instance()->GetConfig(_type);
        AWRAD_SOCRE = config.score;
        MISSION_LAST_TIME = config.time;
        type_ = _type;
    }

    bool Mission::is_timeout()
    {
        time_t now = time(nullptr);
        return (now - startTime_) >= MISSION_LAST_TIME;
    }

    void Mission::pubulish(time_t _time)
    {
        startTime_ = _time;
    }

    void Mission::stop(time_t _time)
    {
        stopTime_ = _time;
    }

    int Mission::settle(SesstleFunction _func)
    {
        return (_func() && ((stopTime_ - startTime_) >= MISSION_LAST_TIME)) ? AWRAD_SOCRE : PENTITY_SCORE;
    }

}