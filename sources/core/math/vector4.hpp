#ifndef STRAITX_VECTOR4_HPP
#define STRAITX_VECTOR4_HPP

#include "core/types.hpp"
#include "core/assert.hpp"
#include "core/printer.hpp"
#include "core/math/vector3.hpp"

template<typename T>
struct Vector4{
    union{
        struct{
            T x;
            T y;
            T z;
            T w;
        };
        T Data[4];
    };

    // Creates a Vector4(0, 0, 0, 0).
    constexpr Vector4();

    constexpr Vector4(const T &XYZW);

    constexpr Vector4(const T &X, const T &Y, const T &Z, const T &W);

    constexpr Vector4(const Vector2<T> &XY, const T &Z, const T &W);

    constexpr Vector4(const Vector3<T> &XYZ, const T &W);

    constexpr Vector4(const Vector4 &other) = default;

    constexpr Vector4(Vector4 &&other) = default;

    template <typename O>
    constexpr Vector4(const Vector4<O> &other);

    constexpr Vector4 &operator=(const Vector4 &other);

    constexpr Vector4 &operator=(Vector4 &&other);    
    // acces elements as if vector was an array
    constexpr T &operator[](size_t index);
    // acces const elements as if vector was an array
    constexpr const T &operator[](size_t index) const;

    constexpr Vector2<T> XY()const;

    constexpr Vector3<T> XYZ()const;
};


template <typename T>
constexpr Vector4<T>::Vector4():
    x(static_cast<T>(0)),
    y(static_cast<T>(0)),
    z(static_cast<T>(0)),
    w(static_cast<T>(0))
{}

template <typename T>
constexpr Vector4<T>::Vector4(const T &XYZW):
    x(XYZW),
    y(XYZW),
    z(XYZW),
    w(XYZW)
{}

template <typename T>
constexpr Vector4<T>::Vector4(const T &X, const T &Y, const T &Z, const T &W):
    x(X),
    y(Y),
    z(Z),
    w(W)
{}

template <typename T>
constexpr Vector4<T>::Vector4(const Vector2<T> &XY, const T &Z, const T &W):
    x(XY.x),
    y(XY.y),
    z(Z),
    w(W)
{}

template <typename T>
constexpr Vector4<T>::Vector4(const Vector3<T> &XYZ, const T &W):
    x(XYZ.x),
    y(XYZ.y),
    z(XYZ.z),
    w(W)
{}

template <typename T>
template <typename O>
constexpr Vector4<T>::Vector4(const Vector4<O> &other):
    x(static_cast<T>(other.x)),
    y(static_cast<T>(other.y)),
    z(static_cast<T>(other.z)),
    w(static_cast<T>(other.w))
{}

template <typename T>
constexpr Vector4<T> &Vector4<T>::operator=(const Vector4<T> &other){
    x=other.x;
    y=other.y;
    z=other.z;
    w=other.w;
    return *this;
}

template <typename T>
constexpr Vector4<T> &Vector4<T>::operator=(Vector4<T> &&other){
    x=other.x;
    y=other.y;
    z=other.z;
    w=other.w;
    return *this;
}

template <typename T>
constexpr T &Vector4<T>::operator[](size_t index){
    SX_CORE_ASSERT(index < 4,"Vector4 can not index more than 4 elements");
    return Data[index];
}

template <typename T>
constexpr const T &Vector4<T>::operator[](size_t index) const{
    return const_cast<Vector4<T>*>(this)->operator[](index);
}

template <typename T>
constexpr Vector2<T> Vector4<T>::XY() const{
    return {x, y};
}

template <typename T>
constexpr Vector3<T> Vector4<T>::XYZ() const{
    return {x, y, z};
}

template <typename T>
constexpr bool operator==(const Vector4<T> &left, const Vector4<T> &right){
    return (left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w);
}

template <typename T>
constexpr bool operator!=(const Vector4<T> &left, const Vector4<T> &right){
    return !(left==right);
}

template <typename T>
constexpr Vector4<T> operator-(const Vector4<T> &right){
    return Vector4<T>(-right.x, -right.y, -right.z, -right.w);
}

template <typename T>
constexpr Vector4<T> operator+(const T &left, const Vector4<T> &right){
    return Vector4<T>(left+right.x, left+right.y, left+right.z, left+right.w); 
}

template <typename T>
constexpr Vector4<T> operator*(const T &left, const Vector4<T> &right){
    return Vector4<T>(left*right.x, left*right.y, left*right.z, left*right.w);
}

template <typename T>
constexpr Vector4<T> operator+(const Vector4<T> &left, const T &right){
    return Vector4<T>(left.x+right, left.y+right, left.z+right, left.w+right);
}

template <typename T>
constexpr Vector4<T> operator-(const Vector4<T> &left, const T &right){
    return Vector4<T>(left.x-right, left.y-right, left.z-right, left.w-right);
}

template <typename T>
constexpr Vector4<T> operator*(const Vector4<T> &left, const T &right){
    return Vector4<T>(left.x*right, left.y*right, left.z*right, left.w*right);
}

template <typename T>
constexpr Vector4<T> operator/(const Vector4<T> &left, const T &right){
    return Vector4<T>(left.x/right, left.y/right, left.z/right, left.w/right);
}

template <typename T>
constexpr Vector4<T> operator+(const Vector4<T> &left, const Vector4<T> &right){
    return Vector4<T>(left.x+right.x, left.y+right.y, left.z+right.z, left.w+right.w);
}

template <typename T>
constexpr Vector4<T> operator-(const Vector4<T> &left, const Vector4<T> &right){
    return Vector4<T>(left.x-right.x, left.y-right.y, left.z-right.z, left.w-right.w);
}

template <typename T>
constexpr Vector4<T> operator*(const Vector4<T> &left, const Vector4<T> &right){
    return Vector4<T>(left.x*right.x, left.y*right.y, left.z*right.z, left.w*right.w);
}

template <typename T>
constexpr Vector4<T> operator/(const Vector4<T> &left, const Vector4<T> &right){
    return Vector4<T>(left.x/right.x, left.y/right.y, left.z/right.z, left.w/right.w);
}

template <typename T>
constexpr Vector4<T> &operator+=(Vector4<T> &left, const T &right){
    left.x+=right;
    left.y+=right;
    left.z+=right;
    left.w+=right;
    return left;
}

template <typename T>
constexpr Vector4<T> &operator-=(Vector4<T> &left, const T &right){
    left.x-=right;
    left.y-=right;
    left.z-=right;
    left.w-=right;
    return left;
}

template <typename T>
constexpr Vector4<T> &operator*=(Vector4<T> &left, const T &right){
    left.x*=right;
    left.y*=right;
    left.z*=right;
    left.w*=right;
    return left;
}

template <typename T>
constexpr Vector4<T> &operator/=(Vector4<T> &left, const T &right){
    left.x/=right;
    left.y/=right;
    left.z/=right;
    left.w/=right;
    return left;
}

template <typename T>
constexpr Vector4<T> &operator+=(Vector4<T> &left, const Vector4<T> &right){
    left.x+=right.x;
    left.y+=right.y;
    left.z+=right.z;
    left.w+=right.w;
    return left;
}

template <typename T>
constexpr Vector4<T> &operator-=(Vector4<T> &left, const Vector4<T> &right){
    left.x-=right.x;
    left.y-=right.y;
    left.z-=right.z;
    left.w-=right.w;
    return left;
}

template <typename T>
constexpr Vector4<T> &operator*=(Vector4<T> &left, const Vector4<T> &right){
    left.x*=right.x;
    left.y*=right.y;
    left.z*=right.z;
    left.w*=right.w;
    return left;
}

template <typename T>
constexpr Vector4<T> &operator/=(Vector4<T> &left, const Vector4<T> &right){
    left.x/=right.x;
    left.y/=right.y;
    left.z/=right.z;
    left.w/=right.w;
    return left;
}

typedef Vector4<s32> Vector4s;
typedef Vector4<u32> Vector4u;
typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;

namespace Math{

template <typename NumberType>
constexpr Vector4<NumberType> Clamp(Vector4<NumberType> vec, NumberType lower, NumberType upper) {
    return {
        Math::Clamp(vec.x, lower, upper),
        Math::Clamp(vec.y, lower, upper),
        Math::Clamp(vec.z, lower, upper),
        Math::Clamp(vec.w, lower, upper)
    };
}

}//namespace Math::

template<typename T>
struct Printer<Vector4<T>>{
	static void Print(const Vector4<T> &value, StringWriter &writer){
		Printer<char>::Print('(', writer);
		Printer<T>::Print(value.x, writer);
		Printer<char>::Print(',', writer);
		Printer<T>::Print(value.y, writer);
		Printer<char>::Print(',', writer);
		Printer<T>::Print(value.z, writer);
		Printer<char>::Print(',', writer);
		Printer<T>::Print(value.w, writer);
		Printer<char>::Print(')', writer);
	}
};

#endif // STRAITX_VECTOR4_HPP