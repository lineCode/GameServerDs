#ifndef MINIPROJECT_Mission_HPP
#define MINIPROJECT_Mission_HPP

#include <time.h>
#include <string>
#include <functional>
#include <memory>
#include "MissionConfig.h"
namespace MiniProject
{
    typedef std::function<bool()> SesstleFunction;

    class Mission
    {
    public:
        typedef std::shared_ptr<Mission> MissionPtr;

    public:
        Mission(MissionType _type);

        bool is_timeout();
        void pubulish(time_t t = time(nullptr));
        void stop(time_t t = time(nullptr));
        virtual int settle(SesstleFunction _func);

        MissionType Type()
        {
            return type_;
        }

        int Time()
        {
            return MISSION_LAST_TIME;
        }

        int GetScore()
        {
            return AWRAD_SOCRE;
        }

        int GetTarget()
        {
            return missionTarget;
        }

    private:
        int AWRAD_SOCRE;
        int PENTITY_SCORE;
        int missionTarget;
        unsigned long long MISSION_LAST_TIME;
        time_t startTime_;
        time_t stopTime_;
        MissionType type_;
    };

}

#endif