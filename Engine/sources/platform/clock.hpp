#ifndef STRAITX_CLOCK_HPP
#define STRAITX_CLOCK_HPP

#include "platform/time.hpp"
#include "platform/compiler.hpp"



class Clock{
private:
    Time m_Epoch;
public:
    Clock();

    Clock(const Clock &other) = default;

    Clock &operator=(const Clock &other) = default;

    Time GetElapsedTime();

    void Restart();
    // platform specific implementation
    static Time GetMonotonicTime();
};


SX_INLINE Clock::Clock(){
    Restart();
}

SX_INLINE Time Clock::GetElapsedTime(){
    return GetMonotonicTime() - m_Epoch;
}

SX_INLINE void Clock::Restart(){
    m_Epoch = GetMonotonicTime();
}



#endif//STRAITX_CLOCK_HPP