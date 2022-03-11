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
constexpr NumberType Tan(NumberType radians){
    return tan(radians);
}

template<typename NumberType>
constexpr NumberType Cot(NumberType radians){
    return Tan(NumberType(Pi/2.0) - radians);
}

template<typename NumberType>
constexpr NumberType Deg(NumberType radians){
    return static_cast<NumberType>(radians / NumberType(Pi) * NumberType(180.0));
}

template<typename NumberType>
constexpr NumberType Rad(NumberType degrees){
    return static_cast<NumberType>(degrees / NumberType(180.0) * NumberType(Pi));
}

template <typename NumberType, typename = decltype(float(*(NumberType*)nullptr))>
float Sqrt(NumberType number) {
    return sqrtf((float)number);
}

SX_INLINE double Sqrt(double number) {
    return sqrt(number);
}

template <typename IntegerType>
constexpr IntegerType AlignUp(IntegerType number, IntegerType alignment) {
    return number + (alignment - number % alignment) % alignment;
}

template <typename IntegerType>
constexpr IntegerType AlignDown(IntegerType number, IntegerType alignment) {
    return number - number % alignment;
}

}//namespace Math::

#endif//STRAITX_TRIG_HPP