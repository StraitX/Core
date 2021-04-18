#ifndef STRAITX_CLOCK_HPP
#define STRAITX_CLOCK_HPP

#include "platform/time.hpp"
#include "platform/compiler.hpp"

namespace StraitX{

class Clock{
private:
    Time m_Epoch;
public:
    Clock();

    Clock(const Clock &other) = default;

    Clock &operator=(const Clock &other) = default;

    Time GetElapsedTime();

    void Restart();
private:
    static Time GetMonotonicTime();
};


sx_inline Clock::Clock(){
    Restart();
}

sx_inline Time Clock::GetElapsedTime(){
    return GetMonotonicTime() - m_Epoch;
}

sx_inline void Clock::Restart(){
    m_Epoch = GetMonotonicTime();
}

}//namespace StraitX::

#endif//STRAITX_CLOCK_HPP