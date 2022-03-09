#ifndef STRAITX_TRANSFORM_HPP
#define STRAITX_TRANSFORM_HPP

#include "core/math/vector3.hpp"
#include "core/math/matrix4.hpp"
#include "core/math/functions.hpp"

namespace Math{

template<typename MatrixType, typename AngleType>
constexpr Matrix4<MatrixType> RotateZ(AngleType radians){
    Matrix4<MatrixType> matrix(static_cast<MatrixType>(1));
    matrix[0][0] = Math::Cos(radians);
    matrix[1][0] = Math::Sin(radians);
    matrix[0][1] =-Math::Sin(radians);
    matrix[1][1] = Math::Cos(radians);
    return matrix;
}

template<typename MatrixType, typename AngleType>
constexpr Matrix4<MatrixType> RotateY(AngleType radians){
    Matrix4<MatrixType> matrix(static_cast<MatrixType>(1));
    matrix[0][0] = Math::Cos(radians);
    matrix[2][0] = Math::Sin(radians);
    matrix[0][2] =-Math::Sin(radians);
    matrix[2][2] = Math::Cos(radians);
    return matrix;
}

template<typename MatrixType, typename AngleType>
constexpr Matrix4<MatrixType> RotateX(AngleType radians){
    Matrix4<MatrixType> matrix(static_cast<MatrixType>(1));
    matrix[1][1] = Math::Cos(radians);
    matrix[2][1] = Math::Sin(radians);
    matrix[1][2] =-Math::Sin(radians);
    matrix[2][2] = Math::Cos(radians);
    return matrix;
}

template<typename MatrixType, typename AngleType>
constexpr Matrix4<MatrixType> Rotate(Vector3<AngleType> radians){
    return RotateX<MatrixType>(radians.x) * RotateY<MatrixType>(radians.y) * RotateZ<MatrixType>(radians.z);
}

}//namespace Math::

#endif//STRAITX_TRANSFORM_HPP
