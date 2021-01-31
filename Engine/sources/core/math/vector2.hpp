#ifndef STRAITX_VECTOR2_HPP
#define STRAITX_VECTOR2_HPP

#include "platform/types.hpp"
#include "core/assert.hpp"
#include "platform/compiler.hpp"

namespace StraitX{

template<typename T>
struct Vector2{
    T x;
    T y;

    // Creates a Vector2(0, 0).
    constexpr Vector2();

    constexpr Vector2(const T &X, const T &Y);

    constexpr Vector2(const Vector2 &other) = default;

    constexpr Vector2(Vector2 &&other) = default;

    template <typename O>
    constexpr explicit Vector2(const Vector2<O> &other);

    constexpr Vector2 &operator=(const Vector2 &other);

    constexpr Vector2 &operator=(Vector2 &&other);    
    // acces elements as if vector was an array
    constexpr T &operator[](size_t index);
    // acces const elements as if vector was an array
    constexpr const T &operator[](size_t index) const;

};


template <typename T>
constexpr Vector2<T>::Vector2():
    x(static_cast<T>(0)),
    y(static_cast<T>(0))
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
    CoreAssert(index < 2,"Vector2 can not index more than 2 elements");
    return *(&x+index);
}

template <typename T>
constexpr const T &Vector2<T>::operator[](size_t index) const{
    return operator[](index);
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

typedef Vector2<int> Vector2i;
typedef Vector2<unsigned int> Vector2u;
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;

}; // namespace StraitX::

namespace StraitX{

    char *BufferPrint(char *buffer, const char &arg);
    char *BufferPrint(char *buffer, const int &arg);
    char *BufferPrint(char *buffer, const unsigned int &arg);
    char *BufferPrint(char *buffer, const float &arg);
    char *BufferPrint(char *buffer, const double &arg);

    template<typename T>
    sx_inline char *BufferPrint(char *buffer, const Vector2<T> &vector){
        return BufferPrint(BufferPrint(BufferPrint(BufferPrint(BufferPrint(buffer,'('),vector.x),','),vector.y),')');
    }
}; // namespace StraitX::


#endif // STRAITX_VECTOR2_HPP