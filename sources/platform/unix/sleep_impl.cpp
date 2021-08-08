#include <time.h>
#include <errno.h>
#include "core/os/sleep.hpp"

static constexpr i64 AsNanoseconds(Time time){
    return time.AsMicroseconds() * 1000;
}

static constexpr Time Second = Milliseconds(1000);

void Sleep(Time time){
    struct timespec desired;
    desired.tv_sec  = AsNanoseconds(time) / AsNanoseconds(Second);
    desired.tv_nsec = AsNanoseconds(time) % AsNanoseconds(Second);

    while(nanosleep(&desired, &desired) == -1 && errno == EINTR){}
}