#ifndef STRAITX_CLOCK_HPP
#define STRAITX_CLOCK_HPP

#include "core/os/time.hpp"
#include "core/env/compiler.hpp"



class Clock{
private:
    Time m_Epoch;
public:
    Clock();

    Clock(const Clock &other) = default;

    Clock &operator=(const Clock &other) = default;

    Time GetElapsedTime();

    Time Restart();
    // platform specific implementation
    static Time GetMonotonicTime();
};


SX_INLINE Clock::Clock(){
    Restart();
}

SX_INLINE Time Clock::GetElapsedTime(){
    return GetMonotonicTime() - m_Epoch;
}

SX_INLINE Time Clock::Restart(){
    auto last_epoch = m_Epoch;
    m_Epoch = GetMonotonicTime();
    return m_Epoch - last_epoch;
}



#endif//STRAITX_CLOCK_HPP