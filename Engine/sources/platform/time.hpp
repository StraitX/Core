#ifndef STRAITX_TIME_HPP
#define STRAITX_TIME_HPP

#include "platform/types.hpp"

class Time{
private:
    i64 m_Microseconds = 0;

    constexpr Time(i64 microseconds);
public:

    constexpr Time() = default;

    constexpr Time(const Time &other) = default;

    constexpr i64 AsMicroseconds();

    constexpr i64 AsMilliseconds();

    constexpr float AsSeconds();

    friend constexpr Time Seconds(float seconds);

    friend constexpr Time Milliseconds(i64 milliseconds);

    friend constexpr Time Microseconds(i64 microseconds);

    constexpr Time operator+(const Time &other);
    
    constexpr Time operator-(const Time &other);

    friend constexpr Time operator-(const Time &other);

    constexpr Time &operator=(const Time &other);

    constexpr Time &operator+=(const Time &other);

    constexpr Time &operator-=(const Time &other);

    constexpr bool operator==(const Time &other);

    constexpr bool operator!=(const Time &other);

    constexpr bool operator<(const Time &other);
    
    constexpr bool operator>(const Time &other);
    
    constexpr bool operator<=(const Time &other);

    constexpr bool operator>=(const Time &other);
};

constexpr Time::Time(i64 microseconds):
    m_Microseconds(microseconds)
{}

constexpr i64 Time::AsMicroseconds(){
    return m_Microseconds;
}

constexpr i64 Time::AsMilliseconds(){
    return AsMicroseconds() / 1000;
}

constexpr float Time::AsSeconds(){
    return AsMicroseconds() / 1000000.f ;
}

constexpr Time Microseconds(i64 microseconds){
    return Time(microseconds);
}

constexpr Time Milliseconds(i64 milliseconds){
    return Microseconds(milliseconds * 1000);
}

constexpr Time Seconds(float seconds){
    return Microseconds(static_cast<i64>(seconds * 1000000));
}

constexpr Time Time::operator+(const Time &other){
    return Time(m_Microseconds+other.m_Microseconds);
}
    
constexpr Time Time::operator-(const Time &other){
    return Time(m_Microseconds-other.m_Microseconds);
}

constexpr Time operator-(const Time &other){
    return Time(-other.m_Microseconds);
}

constexpr Time &Time::operator=(const Time &other){
    m_Microseconds = other.m_Microseconds;
    return *this;
}

constexpr Time &Time::operator+=(const Time &other){
    m_Microseconds += other.m_Microseconds;
    return *this;
}

constexpr Time &Time::operator-=(const Time &other){
    m_Microseconds -= other.m_Microseconds;
    return *this;
}

constexpr bool Time::operator==(const Time &other){
    return m_Microseconds==other.m_Microseconds;
}

constexpr bool Time::operator!=(const Time &other){
    return m_Microseconds!=other.m_Microseconds;
}

constexpr bool Time::operator<(const Time &other){
    return m_Microseconds<other.m_Microseconds;
}
    
constexpr bool Time::operator>(const Time &other){
    return m_Microseconds>other.m_Microseconds;
}
    
constexpr bool Time::operator<=(const Time &other){
    return m_Microseconds<=other.m_Microseconds;
}

constexpr bool Time::operator>=(const Time &other){
    return m_Microseconds>=other.m_Microseconds;
}

#endif // STRAITX_TIME_HPP