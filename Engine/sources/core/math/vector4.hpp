#ifndef STRAITX_VECTOR4_HPP
#define STRAITX_VECTOR4_HPP

#include "platform/types.hpp"
#include "core/assert.hpp"

namespace StraitX{

template<typename T>
struct Vector4{
    T x;
    T y;
    T z;
    T w;

    // Creates a Vector4(0, 0, 0, 0).
    Vector4();

    Vector4(const T &X, const T &Y, const T &Z, const T &W);

    Vector4(const Vector4 &other) = default;

    Vector4(Vector4 &&other) = default;

    template <typename O>
    explicit Vector4(const Vector4<O> &other);

    Vector4 &operator=(const Vector4 &other);

    Vector4 &operator=(Vector4 &&other);    
    // acces elements as if vector was an array
    T &operator[](size_t index);
    // acces const elements as if vector was an array
    const T &operator[](size_t index) const;

};


template <typename T>
sx_inline Vector4<T>::Vector4():
    x(static_cast<T>(0)),
    y(static_cast<T>(0)),
    z(static_cast<T>(0)),
    w(static_cast<T>(0))
{}

template <typename T>
sx_inline Vector4<T>::Vector4(const T &X, const T &Y, const T &Z, const T &W):
    x(X),
    y(Y),
    z(Z),
    w(W)
{}

template <typename T>
template <typename O>
sx_inline Vector4<T>::Vector4(const Vector4<O> &other):
    x(static_cast<T>(other.x)),
    y(static_cast<T>(other.y)),
    z(static_cast<T>(other.z)),
    w(static_cast<T>(other.w))
{}

template <typename T>
sx_inline Vector4<T> &Vector4<T>::operator=(const Vector4<T> &other){
    x=other.x;
    y=other.y;
    z=other.z;
    w=other.w;
    return *this;
}

template <typename T>
sx_inline Vector4<T> &Vector4<T>::operator=(Vector4<T> &&other){
    x=other.x;
    y=other.y;
    z=other.z;
    w=other.w;
    return *this;
}

template <typename T>
sx_inline T &Vector4<T>::operator[](size_t index){
    CoreAssert(index < 4 && index >=0,"Vector4 can not index more than 4 elements");
    return *(&x+index);
}

template <typename T>
sx_inline const T &Vector4<T>::operator[](size_t index) const{
    CoreAssert(index < 4 && index >=0,"Vector4 can not index more than 4 elements");
    return *(&x+index);
}

template <typename T>
sx_inline bool operator==(const Vector4<T> &left, const Vector4<T> &right){
    return (left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w);
}

template <typename T>
sx_inline bool operator!=(const Vector4<T> &left, const Vector4<T> &right){
    return !(left==right);
}

template <typename T>
sx_inline Vector4<T> operator-(const Vector4<T> &right){
    return Vector4<T>(-right.x, -right.y, -right.z, -right.w);
}

template <typename T>
sx_inline Vector4<T> operator+(const T &left, const Vector4<T> &right){
    return Vector4<T>(left+right.x, left+right.y, left+right.z, left+right.w); 
}

template <typename T>
sx_inline Vector4<T> operator*(const T &left, const Vector4<T> &right){
    return Vector4<T>(left*right.x, left*right.y, left*right.z, left*right.w);
}

template <typename T>
sx_inline Vector4<T> operator+(const Vector4<T> &left, const T &right){
    return Vector4<T>(left.x+right, left.y+right, left.z+right, left.w+right);
}

template <typename T>
sx_inline Vector4<T> operator-(const Vector4<T> &left, const T &right){
    return Vector4<T>(left.x-right, left.y-right, left.z-right, left.w-right);
}

template <typename T>
sx_inline Vector4<T> operator*(const Vector4<T> &left, const T &right){
    return Vector4<T>(left.x*right, left.y*right, left.z*right, left.w*right);
}

template <typename T>
sx_inline Vector4<T> operator/(const Vector4<T> &left, const T &right){
    return Vector4<T>(left.x/right, left.y/right, left.z/right, left.w/right);
}

template <typename T>
sx_inline Vector4<T> operator+(const Vector4<T> &left, const Vector4<T> &right){
    return Vector4<T>(left.x+right.x, left.y+right.y, left.z+right.z, left.w+right.w);
}

template <typename T>
sx_inline Vector4<T> operator-(const Vector4<T> &left, const Vector4<T> &right){
    return Vector4<T>(left.x-right.x, left.y-right.y, left.z-right.z, left.w-right.w);
}

template <typename T>
sx_inline Vector4<T> operator*(const Vector4<T> &left, const Vector4<T> &right){
    return Vector4<T>(left.x*right.x, left.y*right.y, left.z*right.z, left.w*right.w);
}

template <typename T>
sx_inline Vector4<T> operator/(const Vector4<T> &left, const Vector4<T> &right){
    return Vector4<T>(left.x/right.x, left.y/right.y, left.z/right.z, left.w/right.w);
}

template <typename T>
sx_inline Vector4<T> &operator+=(Vector4<T> &left, const T &right){
    left.x+=right;
    left.y+=right;
    left.z+=right;
    left.w+=right;
    return left;
}

template <typename T>
sx_inline Vector4<T> &operator-=(Vector4<T> &left, const T &right){
    left.x-=right;
    left.y-=right;
    left.z-=right;
    left.w-=right;
    return left;
}

template <typename T>
sx_inline Vector4<T> &operator*=(Vector4<T> &left, const T &right){
    left.x*=right;
    left.y*=right;
    left.z*=right;
    left.w*=right;
    return left;
}

template <typename T>
sx_inline Vector4<T> &operator/=(Vector4<T> &left, const T &right){
    left.x/=right;
    left.y/=right;
    left.z/=right;
    left.w/=right;
    return left;
}

template <typename T>
sx_inline Vector4<T> &operator+=(Vector4<T> &left, const Vector4<T> &right){
    left.x+=right.x;
    left.y+=right.y;
    left.z+=right.z;
    left.w+=right.w;
    return left;
}

template <typename T>
sx_inline Vector4<T> &operator-=(Vector4<T> &left, const Vector4<T> &right){
    left.x-=right.x;
    left.y-=right.y;
    left.z-=right.z;
    left.w-=right.w;
    return left;
}

template <typename T>
sx_inline Vector4<T> &operator*=(Vector4<T> &left, const Vector4<T> &right){
    left.x*=right.x;
    left.y*=right.y;
    left.z*=right.z;
    left.w*=right.w;
    return left;
}

template <typename T>
sx_inline Vector4<T> &operator/=(Vector4<T> &left, const Vector4<T> &right){
    left.x/=right.x;
    left.y/=right.y;
    left.z/=right.z;
    left.w/=right.w;
    return left;
}

typedef Vector4<int> Vector4i;
typedef Vector4<unsigned int> Vector4u;
typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;

}; // namespace StraitX::

namespace StraitX{

    char *BufferPrint(char *buffer, const char &arg);
    char *BufferPrint(char *buffer, const int &arg);
    char *BufferPrint(char *buffer, const unsigned int &arg);
    char *BufferPrint(char *buffer, const float &arg);
    char *BufferPrint(char *buffer, const double &arg);

    template<typename T>
    sx_inline char *BufferPrint(char *buffer, const Vector4<T> &vector){
        return BufferPrint(BufferPrint(BufferPrint(BufferPrint(BufferPrint(BufferPrint(BufferPrint(BufferPrint(BufferPrint(buffer,'('),vector.x),','),vector.y),','),vector.z),','),vector.w),')');
    }
}; // namespace StraitX::

#endif // STRAITX_VECTOR4_HPP