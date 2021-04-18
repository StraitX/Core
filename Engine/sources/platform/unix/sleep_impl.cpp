#include <time.h>
#include <errno.h>
#include "platform/sleep.hpp"

namespace StraitX{

void Sleep(Time time){
    struct timespec desired;
    desired.tv_sec  = time.AsNanoseconds() / Milliseconds(1000).AsNanoseconds();
    desired.tv_nsec = time.AsNanoseconds() % Milliseconds(1000).AsNanoseconds();

    while(nanosleep(&desired, &desired) == -1 && errno == EINTR){}
}

}//namespace StraitX::