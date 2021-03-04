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

    constexpr Matrix4 GetTransposed();
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
template <typename T>
constexpr Matrix4<T> Matrix4<T>::GetTransposed(){
    return {
        {Rows[0][0], Rows[1][0], Rows[2][0], Rows[3][0]},
        {Rows[0][1], Rows[1][1], Rows[2][1], Rows[3][1]},
        {Rows[0][2], Rows[1][2], Rows[2][2], Rows[3][2]},
        {Rows[0][3], Rows[1][3], Rows[2][3], Rows[3][3]}
    };
}

typedef Matrix4<float> Matrix4f;
typedef Matrix4<s32> Matrix4i;
typedef Matrix4<u32> Matrix4u;    

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