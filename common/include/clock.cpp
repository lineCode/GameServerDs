#include "clock.h"

namespace MiniProject
{

    /* get system time */
    void itimeofday(long *sec, long *usec)
    {
        struct timeval time;
        gettimeofday(&time, NULL);
        if (sec)
            *sec = time.tv_sec;
        if (usec)
            *usec = time.tv_usec;
    }

    /* get clock in millisecond 64 */
    uint64_t iclock64(void)
    {
        long s, u;
        uint64_t value;
        itimeofday(&s, &u);
        value = ((uint64_t)s) * 1000 + (u / 1000);
        return value;
    }

    uint32_t iclock()
    {
        return (uint32_t)(iclock64() & 0xfffffffful);
    }

}