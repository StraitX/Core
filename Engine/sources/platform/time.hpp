#ifndef STRAITX_TIME_HPP
#define STRAITX_TIME_HPP

#include "platform/types.hpp"

namespace StraitX{

class Time{
private:
    s64 m_Nanoseconds = 0;

    constexpr Time(s64 nanoseconds);
public:

    constexpr Time() = default;

    constexpr Time(const Time &other) = default;

    constexpr Time(Time &&other);

    constexpr s64 AsNanoseconds();

    constexpr s64 AsMicroseconds();

    constexpr s64 AsMilliseconds();

    constexpr float AsSeconds();

    friend constexpr Time Seconds(float seconds);

    friend constexpr Time Milliseconds(s64 milliseconds);

    friend constexpr Time Microseconds(s64 microseconds);

    friend constexpr Time Nanoseconds(s64 nanoseconds);

    constexpr Time operator+(const Time &other);
    
    constexpr Time operator-(const Time &other);

    friend constexpr Time operator-(const Time &other);

    constexpr Time &operator=(const Time &other);

    constexpr Time &operator=(Time &&other);

    constexpr Time &operator+=(const Time &other);

    constexpr Time &operator-=(const Time &other);

    constexpr bool operator==(const Time &other);

    constexpr bool operator!=(const Time &other);

    constexpr bool operator<(const Time &other);
    
    constexpr bool operator>(const Time &other);
    
    constexpr bool operator<=(const Time &other);

    constexpr bool operator>=(const Time &other);
};

constexpr Time::Time(s64 nanoseconds):
    m_Nanoseconds(nanoseconds)
{}
    
constexpr Time::Time(Time &&other):
    m_Nanoseconds(other.m_Nanoseconds)
{
    other.m_Nanoseconds= 0;
}


constexpr s64 Time::AsNanoseconds(){
    return m_Nanoseconds;
}

constexpr s64 Time::AsMicroseconds(){
    return AsNanoseconds() / 1000;
}

constexpr s64 Time::AsMilliseconds(){
    return AsMicroseconds() / 1000;
}

constexpr float Time::AsSeconds(){
    return AsMicroseconds() / 1000000.f ;
}

constexpr Time Nanoseconds(s64 nanoseconds){
    return Time(nanoseconds);
}

constexpr Time Microseconds(s64 microseconds){
    return Nanoseconds(microseconds * 1000);
}

constexpr Time Milliseconds(s64 milliseconds){
    return Microseconds(milliseconds * 1000);
}

constexpr Time Seconds(float seconds){
    return Microseconds(static_cast<s64>(seconds * 1000000));
}

constexpr Time Time::operator+(const Time &other){
    return Time(m_Nanoseconds+other.m_Nanoseconds);
}
    
constexpr Time Time::operator-(const Time &other){
    return Time(m_Nanoseconds-other.m_Nanoseconds);
}

constexpr Time operator-(const Time &other){
    return Time(-other.m_Nanoseconds);
}

constexpr Time &Time::operator=(const Time &other){
    m_Nanoseconds = other.m_Nanoseconds;
    return *this;
}

constexpr Time &Time::operator=(Time &&other){
    m_Nanoseconds = other.m_Nanoseconds;
    other.m_Nanoseconds = 0;
    return *this;    
}

constexpr Time &Time::operator+=(const Time &other){
    m_Nanoseconds += other.m_Nanoseconds;
    return *this;
}

constexpr Time &Time::operator-=(const Time &other){
    m_Nanoseconds -= other.m_Nanoseconds;
    return *this;
}

constexpr bool Time::operator==(const Time &other){
    return m_Nanoseconds==other.m_Nanoseconds;
}

constexpr bool Time::operator!=(const Time &other){
    return m_Nanoseconds!=other.m_Nanoseconds;
}

constexpr bool Time::operator<(const Time &other){
    return m_Nanoseconds<other.m_Nanoseconds;
}
    
constexpr bool Time::operator>(const Time &other){
    return m_Nanoseconds>other.m_Nanoseconds;
}
    
constexpr bool Time::operator<=(const Time &other){
    return m_Nanoseconds<=other.m_Nanoseconds;
}

constexpr bool Time::operator>=(const Time &other){
    return m_Nanoseconds>=other.m_Nanoseconds;
}


}; // namespace StraitX::

#endif // STRAITX_TIME_HPP