#ifndef STRAITX_VECTOR3_HPP
#define STRAITX_VECTOR3_HPP

#include "platform/types.hpp"
#include "core/assert.hpp"

namespace StraitX{

template<typename T>
struct Vector3{
    T x;
    T y;
    T z;

    // Creates a Vector3(0, 0, 0).
    Vector3();

    Vector3(const T &X, const T &Y, const T &Z);

    Vector3(const Vector3 &other) = default;

    Vector3(Vector3 &&other) = default;

    template <typename O>
    explicit Vector3(const Vector3<O> &other);

    Vector3 &operator=(const Vector3 &other);

    Vector3 &operator=(Vector3 &&other);    
    // acces elements as if vector was an array
    T &operator[](size_t index);
    // acces const elements as if vector was an array
    const T &operator[](size_t index) const;

};


template <typename T>
sx_inline Vector3<T>::Vector3():
    x(static_cast<T>(0)),
    y(static_cast<T>(0)),
    z(static_cast<T>(0))
{}

template <typename T>
sx_inline Vector3<T>::Vector3(const T &X, const T &Y, const T &Z):
    x(X),
    y(Y),
    z(Z)
{}

template <typename T>
template <typename O>
sx_inline Vector3<T>::Vector3(const Vector3<O> &other):
    x(static_cast<T>(other.x)),
    y(static_cast<T>(other.y)),
    z(static_cast<T>(other.z))
{}

template <typename T>
sx_inline Vector3<T> &Vector3<T>::operator=(const Vector3<T> &other){
    x=other.x;
    y=other.y;
    z=other.z;
    return *this;
}

template <typename T>
sx_inline Vector3<T> &Vector3<T>::operator=(Vector3<T> &&other){
    x=other.x;
    y=other.y;
    z=other.z;
    return *this;
}

template <typename T>
sx_inline T &Vector3<T>::operator[](size_t index){
    CoreAssert(index < 3 && index >=0,"Vector3 can not index more than 3 elements");
    return *(&x+index);
}

template <typename T>
sx_inline const T &Vector3<T>::operator[](size_t index) const{
    CoreAssert(index < 3 && index >=0,"Vector3 can not index more than 3 elements");
    return *(&x+index);
}

template <typename T>
sx_inline bool operator==(const Vector3<T> &left, const Vector3<T> &right){
    return (left.x == right.x && left.y == right.y && left.z == right.z);
}

template <typename T>
sx_inline bool operator!=(const Vector3<T> &left, const Vector3<T> &right){
    return !(left==right);
}

template <typename T>
sx_inline Vector3<T> operator-(const Vector3<T> &right){
    return Vector3<T>(-right.x, -right.y, -right.z);
}

template <typename T>
sx_inline Vector3<T> operator+(const T &left, const Vector3<T> &right){
    return Vector3<T>(left+right.x, left+right.y, left+right.z);
}

template <typename T>
sx_inline Vector3<T> operator*(const T &left, const Vector3<T> &right){
    return Vector3<T>(left*right.x, left*right.y, left*right.z);
}

template <typename T>
sx_inline Vector3<T> operator+(const Vector3<T> &left, const T &right){
    return Vector3<T>(left.x+right, left.y+right, left.z+right);
}

template <typename T>
sx_inline Vector3<T> operator-(const Vector3<T> &left, const T &right){
    return Vector3<T>(left.x-right, left.y-right, left.z-right);
}

template <typename T>
sx_inline Vector3<T> operator*(const Vector3<T> &left, const T &right){
    return Vector3<T>(left.x*right, left.y*right, left.z*right);
}

template <typename T>
sx_inline Vector3<T> operator/(const Vector3<T> &left, const T &right){
    return Vector3<T>(left.x/right, left.y/right, left.z/right);
}

template <typename T>
sx_inline Vector3<T> operator+(const Vector3<T> &left, const Vector3<T> &right){
    return Vector3<T>(left.x+right.x, left.y+right.y, left.z+right.z);
}

template <typename T>
sx_inline Vector3<T> operator-(const Vector3<T> &left, const Vector3<T> &right){
    return Vector3<T>(left.x-right.x, left.y-right.y, left.z-right.z);
}

template <typename T>
sx_inline Vector3<T> operator*(const Vector3<T> &left, const Vector3<T> &right){
    return Vector3<T>(left.x*right.x, left.y*right.y, left.z*right.z);
}

template <typename T>
sx_inline Vector3<T> operator/(const Vector3<T> &left, const Vector3<T> &right){
    return Vector3<T>(left.x/right.x, left.y/right.y, left.z/right.z);
}

template <typename T>
sx_inline Vector3<T> &operator+=(Vector3<T> &left, const T &right){
    left.x+=right;
    left.y+=right;
    left.z+=right;
    return left;
}

template <typename T>
sx_inline Vector3<T> &operator-=(Vector3<T> &left, const T &right){
    left.x-=right;
    left.y-=right;
    left.z-=right;
    return left;
}

template <typename T>
sx_inline Vector3<T> &operator*=(Vector3<T> &left, const T &right){
    left.x*=right;
    left.y*=right;
    left.z*=right;
    return left;
}

template <typename T>
sx_inline Vector3<T> &operator/=(Vector3<T> &left, const T &right){
    left.x/=right;
    left.y/=right;
    left.z/=right;
    return left;
}

template <typename T>
sx_inline Vector3<T> &operator+=(Vector3<T> &left, const Vector3<T> &right){
    left.x+=right.x;
    left.y+=right.y;
    left.z+=right.z;
    return left;
}

template <typename T>
sx_inline Vector3<T> &operator-=(Vector3<T> &left, const Vector3<T> &right){
    left.x-=right.x;
    left.y-=right.y;
    left.z-=right.z;
    return left;
}

template <typename T>
sx_inline Vector3<T> &operator*=(Vector3<T> &left, const Vector3<T> &right){
    left.x*=right.x;
    left.y*=right.y;
    left.z*=right.z;
    return left;
}

template <typename T>
sx_inline Vector3<T> &operator/=(Vector3<T> &left, const Vector3<T> &right){
    left.x/=right.x;
    left.y/=right.y;
    left.z/=right.z;
    return left;
}

typedef Vector3<int> Vector3i;
typedef Vector3<unsigned int> Vector3u;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

}; // namespace StraitX::



namespace StraitX{

    char *BufferPrint(char *buffer, const char &arg);
    char *BufferPrint(char *buffer, const int &arg);
    char *BufferPrint(char *buffer, const unsigned int &arg);
    char *BufferPrint(char *buffer, const float &arg);
    char *BufferPrint(char *buffer, const double &arg);

    template<typename T>
    sx_inline char *BufferPrint(char *buffer, const Vector3<T> &vector){
        return BufferPrint(BufferPrint(BufferPrint(BufferPrint(BufferPrint(BufferPrint(BufferPrint(buffer,'('),vector.x),','),vector.y),','),vector.z),')');
    }
}; // namespace StraitX::

#endif // STRAITX_VECTOR3_HPP