#include <Windows.h>
#include <thread>
#include "core/os/sleep.hpp"

void Sleep(Time time){
    TIMECAPS tc;
    timeGetDevCaps(&tc, sizeof(TIMECAPS));

    timeBeginPeriod(tc.wPeriodMin);

    ::Sleep(time.AsMilliseconds());

    timeEndPeriod(tc.wPeriodMin);
}