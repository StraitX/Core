#include <time.h>
#include "platform/sleep.hpp"

namespace StraitX{

void Sleep(Time time){
    struct timespec desired;
    desired.tv_sec  = time.AsNanoseconds() / Milliseconds(1000).AsNanoseconds();
    desired.tv_nsec = time.AsNanoseconds() % Milliseconds(1000).AsNanoseconds();

    struct timespec remaining = {};

    while(nanosleep(&desired, &remaining) == -1)
        desired = remaining;
}

}//namespace StraitX::