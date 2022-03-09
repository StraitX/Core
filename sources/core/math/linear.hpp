#ifndef STRAITX_LINEAR_HPP
#define STRAITX_LINEAR_HPP

#include "core/math/vector2.hpp"
#include "core/math/vector3.hpp"
#include "core/math/vector4.hpp"

template <typename T>
constexpr T Dot(const Vector2<T> &left, const Vector2<T> &right){
    return left.x * right.x + left.y * right.y;
}

template <typename T>
constexpr T Dot(const Vector3<T> &left, const Vector3<T> &right){
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

template <typename T>
constexpr T Dot(const Vector4<T> &left, const Vector4<T> &right){
    return left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
}

template<typename T>
constexpr T Lerp(const T &f, const T &s, float c){
    return f + (s - f)*c;
}

template<typename T, typename ResultT = decltype(T{} / Vector2<T>{}.Length())>
constexpr Vector2<ResultT> Normalize(const Vector2<T> &vector){
    auto length = vector.Length();

    return Vector2<ResultT>{vector.x / length, vector.y / length};
}

template<typename T, typename ResultT = decltype(T{} / Vector3<T>{}.Length())>
constexpr Vector3<ResultT> Normalize(const Vector3<T> &vector){
    auto length = vector.Length();

    return Vector3<ResultT>{vector.x / length, vector.y / length, vector.z / length};
}


#endif // STRAITX_LINEAR_HPP