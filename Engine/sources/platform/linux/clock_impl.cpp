#include <time.h>
#include "platform/clock.hpp"

namespace StraitX{

Time Clock::GetMonotonicTime(){
    timespec ts{};
    (void)clock_gettime(CLOCK_MONOTONIC, &ts);
    return Nanoseconds(ts.tv_nsec) + Seconds(ts.tv_sec);
}

}//namespace StraitX::