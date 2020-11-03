#ifndef STRAITX_TIME_QUANT_HPP
#define STRAITX_TIME_QUANT_HPP

#include "platform/types.hpp"
#include "platform/compiler.hpp"

namespace StraitX{


class TimeQuant{
private:
    i64 mMicroseconds;

    TimeQuant(const i64 &microseconds);
public:

    TimeQuant();

    TimeQuant(const TimeQuant &other) = default;

    TimeQuant(TimeQuant &&other);

    i64 ToMicroseconds();

    i32 ToMilliseconds();

    float ToSeconds();

    friend TimeQuant Seconds(const float &seconds);

    friend TimeQuant Milliseconds(const i32 &milliseconds);

    friend TimeQuant Microseconds(const i64 &microseconds);

    TimeQuant operator+(const TimeQuant &other);
    
    TimeQuant operator-(const TimeQuant &other);

    friend TimeQuant operator-(const TimeQuant &other);

    TimeQuant &operator=(const TimeQuant &other);

    TimeQuant &operator=(TimeQuant &&other);

    TimeQuant &operator+=(const TimeQuant &other);

    TimeQuant &operator-=(const TimeQuant &other);

    bool operator==(const TimeQuant &other);

    bool operator!=(const TimeQuant &other);

    bool operator<(const TimeQuant &other);
    
    bool operator>(const TimeQuant &other);
    
    bool operator<=(const TimeQuant &other);

    bool operator>=(const TimeQuant &other);
};

TimeQuant Seconds(const float &seconds);

TimeQuant Milliseconds(const i32 &milliseconds);

TimeQuant Microseconds(const i64 &microseconds);


sx_inline TimeQuant TimeQuant::operator+(const TimeQuant &other){
    return TimeQuant(mMicroseconds+other.mMicroseconds);
}
    
sx_inline TimeQuant TimeQuant::operator-(const TimeQuant &other){
    return TimeQuant(mMicroseconds-other.mMicroseconds);
}

sx_inline TimeQuant operator-(const TimeQuant &other){
    return TimeQuant(-other.mMicroseconds);
}

sx_inline TimeQuant &TimeQuant::operator=(const TimeQuant &other){
    mMicroseconds = other.mMicroseconds;
    return *this;
}

sx_inline TimeQuant &TimeQuant::operator=(TimeQuant &&other){
    mMicroseconds = other.mMicroseconds;
    other.mMicroseconds = 0;
    return *this;    
}

sx_inline TimeQuant &TimeQuant::operator+=(const TimeQuant &other){
    mMicroseconds += other.mMicroseconds;
    return *this;
}

sx_inline TimeQuant &TimeQuant::operator-=(const TimeQuant &other){
    mMicroseconds -= other.mMicroseconds;
    return *this;
}

sx_inline bool TimeQuant::operator==(const TimeQuant &other){
    return mMicroseconds==other.mMicroseconds;
}

sx_inline bool TimeQuant::operator!=(const TimeQuant &other){
    return mMicroseconds!=other.mMicroseconds;
}

sx_inline bool TimeQuant::operator<(const TimeQuant &other){
    return mMicroseconds<other.mMicroseconds;
}
    
sx_inline bool TimeQuant::operator>(const TimeQuant &other){
    return mMicroseconds>other.mMicroseconds;
}
    
sx_inline bool TimeQuant::operator<=(const TimeQuant &other){
    return mMicroseconds<=other.mMicroseconds;
}

sx_inline bool TimeQuant::operator>=(const TimeQuant &other){
    return mMicroseconds>=other.mMicroseconds;
}


}; // namespace StraitX::

#endif // STRAITX_TIME_QUANT_HPP