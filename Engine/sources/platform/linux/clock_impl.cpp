#include <time.h>
#include "platform/clock.hpp"

static constexpr Time Nanoseconds(s64 nanoseconds){
    return Microseconds(nanoseconds/1000);
}

Time Clock::GetMonotonicTime(){
    timespec ts{};
    (void)clock_gettime(CLOCK_MONOTONIC, &ts);
    return Nanoseconds(ts.tv_nsec) + Seconds(ts.tv_sec);
}