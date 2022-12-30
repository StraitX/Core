#ifndef STRAITX_MATRIX4_HPP
#define STRAITX_MATRIX4_HPP

#include "core/math/vector4.hpp"
#include "core/assert.hpp"
#include "core/printer.hpp"
#include "core/math/linear.hpp"

// Row Major Matrix
template <typename T>
struct Matrix4{
    Vector4<T> Rows[4];

    // Identity Matrix
    constexpr Matrix4(const T &ident = T(1));

    constexpr Matrix4(const Vector4<T> &row0,
                      const Vector4<T> &row1,
                      const Vector4<T> &row2,
                      const Vector4<T> &row3);
    
    constexpr Vector4<T> &operator[](size_t index);

    constexpr const Vector4<T> &operator[](size_t index)const;

    constexpr Matrix4 GetTransposed()const;

    constexpr Matrix4 GetInverse()const;

    constexpr Vector4<T> Row(size_t index)const;

    constexpr Vector4<T> Column(size_t index)const;

    constexpr T& Flat(size_t index);

    constexpr const T& Flat(size_t index)const;
};

template <typename T>
constexpr Matrix4<T>::Matrix4(const T &i):
    Rows{{i,0,0,0},
         {0,i,0,0},
         {0,0,i,0},
         {0,0,0,i}}
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
constexpr Vector4<T> &Matrix4<T>::operator[](size_t index){
    return const_cast<Vector4<T>&>(const_cast<const Matrix4<T>*>(this)->operator[](index));
}

template <typename T>
constexpr const Vector4<T> &Matrix4<T>::operator[](size_t index)const{
    SX_CORE_ASSERT(index < 4 && index >= 0, "Matrix4: Can't address move than 4 rows"); 
    return Rows[index];
}
template <typename T>
constexpr Matrix4<T> Matrix4<T>::GetTransposed()const{
    return {
        {Rows[0][0], Rows[1][0], Rows[2][0], Rows[3][0]},
        {Rows[0][1], Rows[1][1], Rows[2][1], Rows[3][1]},
        {Rows[0][2], Rows[1][2], Rows[2][2], Rows[3][2]},
        {Rows[0][3], Rows[1][3], Rows[2][3], Rows[3][3]}
    };
}

template <typename T>
constexpr Matrix4<T> Matrix4<T>::GetInverse()const {
    T inv[16];
    T det{};
    int i;

    struct FlatMatrix {
        Matrix4<T> Matrix;

        T& operator[](size_t index) {
            return Matrix.Flat(index);
        }
    };

    FlatMatrix m{ *this };

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return {};

    det = 1.0 / det;
    
    Matrix4<T> result;
    for (i = 0; i < 16; i++)
        result.Flat(i) = inv[i] * det;
    return result;
}

template <typename T>
constexpr Vector4<T> Matrix4<T>::Column(size_t index) const{
    return { Rows[0][index], Rows[1][index], Rows[2][index], Rows[3][index] };
}

template <typename T>
constexpr Vector4<T> Matrix4<T>::Row(size_t index) const{
    return Rows[index];
}

template <typename T>
constexpr const T &Matrix4<T>::Flat(size_t index) const{
    return Rows[index/4][index % 4];
}
template <typename T>
constexpr T &Matrix4<T>::Flat(size_t index) {
    return Rows[index/4][index % 4];
}

template <typename T>
constexpr Matrix4<T> operator*(const Matrix4<T> &l, const Matrix4<T> &r){
    Matrix4<T> res{T{}};
    for(size_t i = 0; i<4; i++)
        for(size_t j = 0; j<4; j++)
            res[i][j] = Dot(l.Row(i), r.Column(j));    
    return res;
}

template <typename T>
constexpr Vector4<T> operator*(const Matrix4<T> &l, const Vector4<T> &r){
    return {
        Dot(l.Row(0), r),
        Dot(l.Row(1), r),
        Dot(l.Row(2), r),
        Dot(l.Row(3), r)
    };
}

typedef Matrix4<float> Matrix4f;
typedef Matrix4<s32> Matrix4s;
typedef Matrix4<u32> Matrix4u;    


template<typename T>
struct Printer<Matrix4<T>>{
	static void Print(const Matrix4<T> &value, StringWriter &writer){
		Printer<char>::Print('\n', writer);
		Printer<Vector4<T>>::Print(value[0], writer);
		Printer<char>::Print('\n', writer);
		Printer<Vector4<T>>::Print(value[1], writer);
		Printer<char>::Print('\n', writer);
		Printer<Vector4<T>>::Print(value[2], writer);
		Printer<char>::Print('\n', writer);
		Printer<Vector4<T>>::Print(value[3], writer);
	}
};


#endif // STRAITX_MATRIX4_HPP