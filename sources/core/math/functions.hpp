#ifndef STRAITX_TRIG_HPP
#define STRAITX_TRIG_HPP

#include <cmath>
#include <float.h>
#include "core/env/compiler.hpp"

namespace Math{

constexpr double Pi = 3.14159265368979323;

template<typename NumberType>
constexpr NumberType Epsilon() {
    static_assert(false, "Epsilon is not implemented for this type");
    return NumberType();
}

template<>
constexpr float Epsilon<float>() {
    return FLT_EPSILON;
}

template<typename NumberType>
constexpr NumberType Abs(NumberType radians){
    return std::abs(radians);
}

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
constexpr NumberType Asin(NumberType radians){
    return asin(radians);
}

template<typename NumberType>
constexpr NumberType Acos(NumberType radians){
    return acos(radians);
}
template<typename NumberType>
constexpr NumberType Atan(NumberType radians){
    return atan(radians);
}
template<typename NumberType>
constexpr NumberType Atan2(NumberType y_radians, NumberType x_radians){
    return atan2(y_radians, x_radians);
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

template <typename NumberType>
constexpr NumberType Clamp(NumberType number, NumberType lower, NumberType upper) {
    if(number > upper)
        return upper;
    if(number < lower)
        return lower;
    return number;
}

}//namespace Math::

#endif//STRAITX_TRIG_HPP