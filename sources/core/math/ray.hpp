#ifndef STRAITX_RAY_HPP
#define STRAITX_RAY_HPP

#include "core/math/vector2.hpp"
#include "core/math/vector3.hpp"

template <typename VectorType, typename ArgType>
class Ray {
private:
    VectorType m_Origin;
    VectorType m_Direction;
public:
    Ray(VectorType origin, VectorType direction):
        m_Origin(origin),
        m_Direction(direction)
    {}

    const VectorType &Origin()const{
        return m_Origin;
    }

    const VectorType& Direction()const {
        return m_Direction;
    }

    ArgType At(ArgType arg)const{
        return m_Origin + m_Direction * arg;
    }
};

using Ray2f = Ray<Vector2f, float>;
using Ray3f = Ray<Vector3f, float>;


#endif//STRAITX_RAY_HPP