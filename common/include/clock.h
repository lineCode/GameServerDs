#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <cstdlib>
#include <iostream>
#include <stddef.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

namespace MiniProject
{

    /* get system time */
    void itimeofday(long *sec, long *usec);
    /* get clock in millisecond 64 */
    uint64_t iclock64(void);

    uint32_t iclock();

}

#endif
