#include <Windows.h>
#include <thread>
#include "platform/sleep.hpp"

void Sleep(Time time){
    TIMECAPS tc;
    timeGetDevCaps(&tc, sizeof(TIMECAPS));

    timeBeginPeriod(tc.wPeriodMin);

    ::Sleep(time.AsMilliseconds());

    timeEndPeriod(tc.wPeriodMin);
}