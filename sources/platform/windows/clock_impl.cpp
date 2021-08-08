#include <Windows.h>
#include "core/os/clock.hpp"

namespace Windows {

LARGE_INTEGER GetFrequency() {
    LARGE_INTEGER frequency = {};
    QueryPerformanceFrequency(&frequency);
    return frequency;
}

}//namespace Windows::

Time Clock::GetMonotonicTime(){
    static double sec_by_freq = 1000000.0 / Windows::GetFrequency().QuadPart;

    LARGE_INTEGER counter = {};
    QueryPerformanceCounter(&counter);

    return Microseconds(counter.QuadPart * sec_by_freq);
}