#include <time.h>
#include "core/os/clock.hpp"

static constexpr Time Nanoseconds(i64 nanoseconds){
    return Microseconds(nanoseconds/1000);
}

Time Clock::GetMonotonicTime(){
    timespec ts{};
    (void)clock_gettime(CLOCK_MONOTONIC, &ts);
    return Nanoseconds(ts.tv_nsec) + Seconds(ts.tv_sec);
}