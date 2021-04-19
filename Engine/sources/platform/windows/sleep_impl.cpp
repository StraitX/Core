#include <Windows.h>
#include <thread>
#include "platform/sleep.hpp"

namespace StraitX{

void Sleep(Time time){
    TIMECAPS tc;
    timeGetDevCaps(&tc, sizeof(TIMECAPS));

    timeBeginPeriod(tc.wPeriodMin);

    ::Sleep(time.AsMilliseconds());

    timeEndPeriod(tc.wPeriodMin);
}

}//namespace StraitX::