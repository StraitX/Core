#ifndef STRAITX_LINEAR_HPP
#define STRAITX_LINEAR_HPP

#include "core/math/vector2.hpp"
#include "core/math/vector3.hpp"
#include "core/math/vector4.hpp"

namespace StraitX{


template <typename T>
constexpr T dot(const Vector2<T> &left, const Vector2<T> &right){
    return left.x * right.x + left.y * right.y;
}

template <typename T>
constexpr T dot(const Vector3<T> &left, const Vector3<T> &right){
    return left.x * right.x + left.y * right.y + left.z * right.z;
}

template <typename T>
constexpr sx_inline T dot(const Vector4<T> &left, const Vector4<T> &right){
    return left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
}


}; // namespace StraitX::


#endif // STRAITX_LINEAR_HPP