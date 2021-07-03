#include <mach/clock.h>
#include <mach/mach_time.h>
#include "platform/clock.hpp"

namespace MacOS {

static mach_timebase_info_data_t GetTimebaseInfoData(){
    mach_timebase_info_data_t info = {0, 0};
    mach_timebase_info(&info);
    return info;
}

}//namespace MacOS::

Time Clock::GetMonotonicTime(){
    static auto freq = MacOS::GetTimebaseInfoData();

    return Microseconds(mach_absolute_time() * freq.numer / freq.denom / 1000);
}