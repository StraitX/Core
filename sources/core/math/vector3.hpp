#ifndef STRAITX_VECTOR3_HPP
#define STRAITX_VECTOR3_HPP

#include "core/types.hpp"
#include "core/assert.hpp"
#include "core/printer.hpp"
#include "core/math/vector2.hpp"

template<typename T>
struct Vector3{
    union {
        struct{
            T x;
            T y;
            T z;
        };
        T Data[3];
    };

    // Creates a Vector3(0, 0, 0).
    constexpr Vector3();

    constexpr Vector3(const T &XYZ);
    
    constexpr Vector3(const Vector2<T> &xy, const T &z);

    constexpr Vector3(const T &X, const T &Y, const T &Z);

    constexpr Vector3(const Vector3 &other) = default;

    constexpr Vector3(Vector3 &&other) = default;

    template <typename O>
    constexpr Vector3(const Vector3<O> &other);

    constexpr Vector3 &operator=(const Vector3 &other);

    constexpr Vector3 &operator=(Vector3 &&other);    
    // acces elements as if vector was an array
    constexpr T &operator[](size_t index);
    // acces const elements as if vector was an array
    constexpr const T &operator[](size_t index) const;

    constexpr Vector2<T> XY()const;

    template <typename LengthType = decltype(Math::Sqrt(T{}*T{}))>
    constexpr LengthType Length()const {
        return Math::Sqrt(x*x + y*y + z*z);
    }
};


template <typename T>
constexpr Vector3<T>::Vector3():
    x(static_cast<T>(0)),
    y(static_cast<T>(0)),
    z(static_cast<T>(0))
{}

template <typename T>
constexpr Vector3<T>::Vector3(const T &XYZ):
    x(XYZ),
    y(XYZ),
    z(XYZ)
{}

template <typename T>
constexpr Vector3<T>::Vector3(const T &X, const T &Y, const T &Z):
    x(X),
    y(Y),
    z(Z)
{}

template <typename T>
constexpr Vector3<T>::Vector3(const Vector2<T> &XY, const T &Z):
    x(XY.x),
    y(XY.y),
    z(Z)
{}

template <typename T>
template <typename O>
constexpr Vector3<T>::Vector3(const Vector3<O> &other):
    x(static_cast<T>(other.x)),
    y(static_cast<T>(other.y)),
    z(static_cast<T>(other.z))
{}

template <typename T>
constexpr Vector3<T> &Vector3<T>::operator=(const Vector3<T> &other){
    x=other.x;
    y=other.y;
    z=other.z;
    return *this;
}

template <typename T>
constexpr Vector3<T> &Vector3<T>::operator=(Vector3<T> &&other){
    x=other.x;
    y=other.y;
    z=other.z;
    return *this;
}

template <typename T>
constexpr T &Vector3<T>::operator[](size_t index){
    SX_CORE_ASSERT(index < 3, "Vector3 can not index more than 3 elements");
    return Data[index];
}

template <typename T>
constexpr const T &Vector3<T>::operator[](size_t index) const{
    return const_cast<Vector3<T>*>(this)->operator[](index);
}

template <typename T>
constexpr Vector2<T> Vector3<T>::XY() const{
    return {x, y};
}

template <typename T>
constexpr bool operator==(const Vector3<T> &left, const Vector3<T> &right){
    return (left.x == right.x && left.y == right.y && left.z == right.z);
}

template <typename T>
constexpr bool operator!=(const Vector3<T> &left, const Vector3<T> &right){
    return !(left==right);
}

template <typename T>
constexpr Vector3<T> operator-(const Vector3<T> &right){
    return Vector3<T>(-right.x, -right.y, -right.z);
}

template <typename T>
constexpr Vector3<T> operator+(const T &left, const Vector3<T> &right){
    return Vector3<T>(left+right.x, left+right.y, left+right.z);
}

template <typename T>
constexpr Vector3<T> operator*(const T &left, const Vector3<T> &right){
    return Vector3<T>(left*right.x, left*right.y, left*right.z);
}

template <typename T>
constexpr Vector3<T> operator+(const Vector3<T> &left, const T &right){
    return Vector3<T>(left.x+right, left.y+right, left.z+right);
}

template <typename T>
constexpr Vector3<T> operator-(const Vector3<T> &left, const T &right){
    return Vector3<T>(left.x-right, left.y-right, left.z-right);
}

template <typename T>
constexpr Vector3<T> operator*(const Vector3<T> &left, const T &right){
    return Vector3<T>(left.x*right, left.y*right, left.z*right);
}

template <typename T>
constexpr Vector3<T> operator/(const Vector3<T> &left, const T &right){
    return Vector3<T>(left.x/right, left.y/right, left.z/right);
}

template <typename T>
constexpr Vector3<T> operator+(const Vector3<T> &left, const Vector3<T> &right){
    return Vector3<T>(left.x+right.x, left.y+right.y, left.z+right.z);
}

template <typename T>
constexpr Vector3<T> operator-(const Vector3<T> &left, const Vector3<T> &right){
    return Vector3<T>(left.x-right.x, left.y-right.y, left.z-right.z);
}

template <typename T>
constexpr Vector3<T> operator*(const Vector3<T> &left, const Vector3<T> &right){
    return Vector3<T>(left.x*right.x, left.y*right.y, left.z*right.z);
}

template <typename T>
constexpr Vector3<T> operator/(const Vector3<T> &left, const Vector3<T> &right){
    return Vector3<T>(left.x/right.x, left.y/right.y, left.z/right.z);
}

template <typename T>
constexpr Vector3<T> &operator+=(Vector3<T> &left, const T &right){
    left.x+=right;
    left.y+=right;
    left.z+=right;
    return left;
}

template <typename T>
constexpr Vector3<T> &operator-=(Vector3<T> &left, const T &right){
    left.x-=right;
    left.y-=right;
    left.z-=right;
    return left;
}

template <typename T>
constexpr Vector3<T> &operator*=(Vector3<T> &left, const T &right){
    left.x*=right;
    left.y*=right;
    left.z*=right;
    return left;
}

template <typename T>
constexpr Vector3<T> &operator/=(Vector3<T> &left, const T &right){
    left.x/=right;
    left.y/=right;
    left.z/=right;
    return left;
}

template <typename T>
constexpr Vector3<T> &operator+=(Vector3<T> &left, const Vector3<T> &right){
    left.x+=right.x;
    left.y+=right.y;
    left.z+=right.z;
    return left;
}

template <typename T>
constexpr Vector3<T> &operator-=(Vector3<T> &left, const Vector3<T> &right){
    left.x-=right.x;
    left.y-=right.y;
    left.z-=right.z;
    return left;
}

template <typename T>
constexpr Vector3<T> &operator*=(Vector3<T> &left, const Vector3<T> &right){
    left.x*=right.x;
    left.y*=right.y;
    left.z*=right.z;
    return left;
}

template <typename T>
constexpr Vector3<T> &operator/=(Vector3<T> &left, const Vector3<T> &right){
    left.x/=right.x;
    left.y/=right.y;
    left.z/=right.z;
    return left;
}

typedef Vector3<s32> Vector3s;
typedef Vector3<u32> Vector3u;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

namespace Math{

template <typename NumberType>
constexpr Vector3<NumberType> Clamp(Vector3<NumberType> vec, NumberType lower, NumberType upper) {
    return {
        Math::Clamp(vec.x, lower, upper),
        Math::Clamp(vec.y, lower, upper),
        Math::Clamp(vec.z, lower, upper)
    };
}

}//namespace Math::

template<typename T>
struct Printer<Vector3<T>>{
	static void Print(const Vector3<T> &value, StringWriter &writer){
		Printer<char>::Print('(', writer);
		Printer<T>::Print(value.x, writer);
		Printer<char>::Print(',', writer);
		Printer<T>::Print(value.y, writer);
		Printer<char>::Print(',', writer);
		Printer<T>::Print(value.z, writer);
		Printer<char>::Print(')', writer);
	}
};

#endif // STRAITX_VECTOR3_HPP