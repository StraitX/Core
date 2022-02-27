#ifndef STRAITX_TRIG_HPP
#define STRAITX_TRIG_HPP

#include <math.h>
#include "core/env/compiler.hpp"

namespace Math{

constexpr double Pi = 3.14159265368979323;

template<typename NumberType>
constexpr NumberType Sin(NumberType radians){
    return sin(radians);
}

template<typename NumberType>
constexpr NumberType Cos(NumberType radians){
    return cos(radians);
}

template<typename NumberType>
constexpr NumberType Deg(NumberType radians){
    return static_cast<NumberType>(radians / Pi * 180.f);
}

template<typename NumberType>
constexpr NumberType Rad(NumberType degrees){
    return static_cast<NumberType>(degrees / 180.f * Pi);
}

SX_INLINE double Sqrt(double number) {
    return sqrt(number);
}

SX_INLINE float Sqrt(float number) {
    return sqrtf(number);
}

}//namespace Math::

#endif//STRAITX_TRIG_HPP