#include "core/time_quant.hpp"


namespace StraitX{

TimeQuant::TimeQuant(const i64 &microseconds):
    mMicroseconds(microseconds)
{}
    

TimeQuant::TimeQuant():
    mMicroseconds(0)
{}

TimeQuant::TimeQuant(TimeQuant &&other):
    mMicroseconds(other.mMicroseconds)
{
    other.mMicroseconds = 0;
}

i64 TimeQuant::ToMicroseconds(){
    return mMicroseconds;
}

i32 TimeQuant::ToMilliseconds(){
    return static_cast<i32>(mMicroseconds/1000);
}

float TimeQuant::ToSeconds(){
    return static_cast<float>(mMicroseconds)/1000000.f;
}


TimeQuant Seconds(const float &seconds){
    return TimeQuant(static_cast<i64>(seconds*1000000.f));
}

TimeQuant Milliseconds(const i32 &milliseconds){
    return TimeQuant(static_cast<i32>(milliseconds*1000));
}

TimeQuant Microseconds(const i64 &microseconds){
    return TimeQuant(microseconds);
}

}; // namespace StraitX::