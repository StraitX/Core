#ifndef STRAITX_MATRIX4_HPP
#define STRAITX_MATRIX4_HPP

#include "core/math/vector4.hpp"
#include "core/assert.hpp"

namespace StraitX{

// Row Major Matrix
template <typename T>
struct Matrix4{
    Vector4<T> Rows[4];

    // Identity Matrix
    constexpr Matrix4();

    constexpr Matrix4(const Vector4<T> &row0,
                      const Vector4<T> &row1,
                      const Vector4<T> &row2,
                      const Vector4<T> &row3);
    
    Vector4<T> &operator[](size_t index);

    const Vector4<T> &operator[](size_t index)const;

};

template <typename T>
constexpr Matrix4<T>::Matrix4():
    Rows{{1,0,0,0},
         {0,1,0,0},
         {0,0,1,0},
         {0,0,0,1}}
{}

template <typename T>
constexpr Matrix4<T>::Matrix4(
    const Vector4<T> &row0,
    const Vector4<T> &row1,
    const Vector4<T> &row2,
    const Vector4<T> &row3):
        Rows{row0, row1, row2, row3}
{}

template<typename T>
sx_inline Vector4<T> &Matrix4<T>::operator[](size_t index){
    CoreAssert(index < 4 && index >= 0, "Matrix4: Can't address move than 4 rows");
    return Rows[index];
}

template <typename T>
sx_inline const Vector4<T> &Matrix4<T>::operator[](size_t index)const{
    CoreAssert(index < 4 && index >= 0, "Matrix4: Can't address move than 4 rows"); 
    return Rows[index];
}


typedef Matrix4<float> Matrix4f;
typedef Matrix4<int> Matrix4i;
typedef Matrix4<unsigned int> Matrix4u;    

}// namepsace StraitX::

namespace StraitX{

template <typename T>
inline char *BufferPrint(char *buffer, const Matrix4<T> &matrix){
    buffer = BufferPrint(buffer, '\n');
    buffer = BufferPrint(buffer, matrix[0]);
    buffer = BufferPrint(buffer, '\n');
    buffer = BufferPrint(buffer, matrix[1]);
    buffer = BufferPrint(buffer, '\n');
    buffer = BufferPrint(buffer, matrix[2]);
    buffer = BufferPrint(buffer, '\n');
    buffer = BufferPrint(buffer, matrix[3]);
    return buffer;
}

}// namespace StraitX::

#endif // STRAITX_MATRIX4_HPP