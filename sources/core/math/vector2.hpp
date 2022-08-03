#ifndef STRAITX_VECTOR2_HPP
#define STRAITX_VECTOR2_HPP

#include "core/types.hpp"
#include "core/assert.hpp"
#include "core/printer.hpp"
#include "core/env/compiler.hpp"
#include "core/math/functions.hpp"

template<typename T>
struct Vector2{
    union{
        struct{
            T x;
            T y;
        };
        T Data[2];
    };

    // Creates a Vector2(0, 0).
    constexpr Vector2();
    
    constexpr Vector2(const T &XY);

    constexpr Vector2(const T &X, const T &Y);

    constexpr Vector2(const Vector2 &other) = default;

    constexpr Vector2(Vector2 &&other) = default;

    template <typename O>
    constexpr Vector2(const Vector2<O> &other);

    constexpr Vector2 &operator=(const Vector2 &other);

    constexpr Vector2 &operator=(Vector2 &&other);    
    // acces elements as if vector was an array
    constexpr T &operator[](size_t index);
    // acces const elements as if vector was an array
    constexpr const T &operator[](size_t index) const;
    
    template <typename LengthType = decltype(Math::Sqrt(T{}*T{}))>
    constexpr LengthType Length()const {
        return Math::Sqrt(x*x + y*y);
    }
};


template <typename T>
constexpr Vector2<T>::Vector2():
    x(static_cast<T>(0)),
    y(static_cast<T>(0))
{}

template <typename T>
constexpr Vector2<T>::Vector2(const T &XY):
    x(XY),
    y(XY)
{}

template <typename T>
constexpr Vector2<T>::Vector2(const T &X, const T &Y):
    x(X),
    y(Y)
{}

template <typename T>
template <typename O>
constexpr Vector2<T>::Vector2(const Vector2<O> &other):
    x(static_cast<T>(other.x)),
    y(static_cast<T>(other.y))
{}

template <typename T>
constexpr Vector2<T> &Vector2<T>::operator=(const Vector2<T> &other){
    x=other.x;
    y=other.y;
    return *this;
}

template <typename T>
constexpr Vector2<T> &Vector2<T>::operator=(Vector2<T> &&other){
    x=other.x;
    y=other.y;
    return *this;
}

template <typename T>
constexpr T &Vector2<T>::operator[](size_t index){
    SX_CORE_ASSERT(index < 2,"Vector2 can not index more than 2 elements");
    return Data[index];
}

template <typename T>
constexpr const T &Vector2<T>::operator[](size_t index) const{
    return const_cast<Vector2<T>*>(this)->operator[](index);
}

template <typename T>
constexpr bool operator==(const Vector2<T> &left, const Vector2<T> &right){
    return (left.x == right.x && left.y == right.y);
}

template <typename T>
constexpr bool operator!=(const Vector2<T> &left, const Vector2<T> &right){
    return !(left==right);
}

template <typename T>
constexpr Vector2<T> operator-(const Vector2<T> &right){
    return Vector2<T>(-right.x, -right.y);
}

template <typename T>
constexpr Vector2<T> operator+(const T &left, const Vector2<T> &right){
    return Vector2<T>(left+right.x, left+right.y);
}

template <typename T>
constexpr Vector2<T> operator*(const T &left, const Vector2<T> &right){
    return Vector2<T>(left*right.x, left*right.y);
}

template <typename T>
constexpr Vector2<T> operator+(const Vector2<T> &left, const T &right){
    return Vector2<T>(left.x+right, left.y+right);
}

template <typename T>
constexpr Vector2<T> operator-(const Vector2<T> &left, const T &right){
    return Vector2<T>(left.x-right, left.y-right);
}

template <typename T>
constexpr Vector2<T> operator*(const Vector2<T> &left, const T &right){
    return Vector2<T>(left.x*right, left.y*right);
}

template <typename T>
constexpr Vector2<T> operator/(const Vector2<T> &left, const T &right){
    return Vector2<T>(left.x/right, left.y/right);
}

template <typename T>
constexpr Vector2<T> operator+(const Vector2<T> &left, const Vector2<T> &right){
    return Vector2<T>(left.x+right.x, left.y+right.y);
}

template <typename T>
constexpr Vector2<T> operator-(const Vector2<T> &left, const Vector2<T> &right){
    return Vector2<T>(left.x-right.x, left.y-right.y);
}

template <typename T>
constexpr Vector2<T> operator*(const Vector2<T> &left, const Vector2<T> &right){
    return Vector2<T>(left.x*right.x, left.y*right.y);
}

template <typename T>
constexpr Vector2<T> operator/(const Vector2<T> &left, const Vector2<T> &right){
    return Vector2<T>(left.x/right.x, left.y/right.y);
}

template <typename T>
constexpr Vector2<T> &operator+=(Vector2<T> &left, const T &right){
    left.x+=right;
    left.y+=right;
    return left;
}

template <typename T>
constexpr Vector2<T> &operator-=(Vector2<T> &left, const T &right){
    left.x-=right;
    left.y-=right;
    return left;
}

template <typename T>
constexpr Vector2<T> &operator*=(Vector2<T> &left, const T &right){
    left.x*=right;
    left.y*=right;
    return left;
}

template <typename T>
constexpr Vector2<T> &operator/=(Vector2<T> &left, const T &right){
    left.x/=right;
    left.y/=right;
    return left;
}

template <typename T>
constexpr Vector2<T> &operator+=(Vector2<T> &left, const Vector2<T> &right){
    left.x+=right.x;
    left.y+=right.y;
    return left;
}

template <typename T>
constexpr Vector2<T> &operator-=(Vector2<T> &left, const Vector2<T> &right){
    left.x-=right.x;
    left.y-=right.y;
    return left;
}

template <typename T>
constexpr Vector2<T> &operator*=(Vector2<T> &left, const Vector2<T> &right){
    left.x*=right.x;
    left.y*=right.y;
    return left;
}

template <typename T>
constexpr Vector2<T> &operator/=(Vector2<T> &left, const Vector2<T> &right){
    left.x/=right.x;
    left.y/=right.y;
    return left;
}

typedef Vector2<s32> Vector2s;
typedef Vector2<u32> Vector2u;
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;

namespace Math{

template <typename NumberType>
constexpr Vector2<NumberType> Clamp(Vector2<NumberType> vec, NumberType lower, NumberType upper) {
    return {
        Math::Clamp(vec.x, lower, upper),
        Math::Clamp(vec.y, lower, upper)
    };
}

}//namespace Math::


template<typename T>
struct Printer<Vector2<T>>{
	static void Print(const Vector2<T> &value, StringWriter &writer){
		Printer<char>::Print('(', writer);
		Printer<T>::Print(value.x, writer);
		Printer<char>::Print(',', writer);
		Printer<T>::Print(value.y, writer);
		Printer<char>::Print(')', writer);
	}
};

#endif // STRAITX_VECTOR2_HPP