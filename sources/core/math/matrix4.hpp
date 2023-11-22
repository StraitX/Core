#ifndef STRAITX_MATRIX4_HPP
#define STRAITX_MATRIX4_HPP

#include "core/math/vector3.hpp"
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
    
    constexpr void Decompose(Vector3<T>& position, Vector3<T>& rotation, Vector3<T>& scale)const;

    constexpr Vector4<T> Row(size_t index)const;

    constexpr Vector4<T> Column(size_t index)const;

    constexpr T& Flat(size_t index);

    constexpr const T& Flat(size_t index)const;

    constexpr T GetDeterminant()const;
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
    const T det = GetDeterminant();

    if(det == static_cast<T>(0))
        return Matrix4<T>();

    const T invdet = static_cast<T>(1.0) / det;
    const Matrix4<T>& m = *this;

    Matrix4<T> res;
    res[0][0] = invdet  * (m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[1][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
    res[0][1] = -invdet * (m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[0][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
    res[0][2] = invdet  * (m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] * (m[1][3] * m[3][1] - m[1][1] * m[3][3]) + m[0][3] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]));
    res[0][3] = -invdet * (m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] * (m[1][3] * m[2][1] - m[1][1] * m[2][3]) + m[0][3] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]));
    res[1][0] = -invdet * (m[1][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
    res[1][1] = invdet  * (m[0][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
    res[1][2] = -invdet * (m[0][0] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][2] - m[1][2] * m[3][0]));
    res[1][3] = invdet  * (m[0][0] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]));
    res[2][0] = invdet  * (m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[1][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
    res[2][1] = -invdet * (m[0][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[0][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
    res[2][2] = invdet  * (m[0][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) + m[0][1] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
    res[2][3] = -invdet * (m[0][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]) + m[0][1] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
    res[3][0] = -invdet * (m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[1][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[1][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
    res[3][1] = invdet  * (m[0][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[0][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[0][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
    res[3][2] = -invdet * (m[0][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) + m[0][1] * (m[1][2] * m[3][0] - m[1][0] * m[3][2]) + m[0][2] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
    res[3][3] = invdet  * (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) + m[0][1] * (m[1][2] * m[2][0] - m[1][0] * m[2][2]) + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));

    return *this;
}

template <typename T>
constexpr void Matrix4<T>::Decompose(Vector3<T>& position, Vector3<T>& rotation, Vector3<T>& scale)const{
	position.x = Rows[0][3]; 
	position.y = Rows[1][3]; 
	position.z = Rows[2][3]; 

	Vector3<T> cols[3] = { 
		Vector3<T>(Rows[0][0], Rows[1][0], Rows[2][0]), 
		Vector3<T>(Rows[0][1], Rows[1][1], Rows[2][1]), 
		Vector3<T>(Rows[0][2], Rows[1][2], Rows[2][2]) 
	}; 

	scale.x = cols[0].Length(); 
	scale.y = cols[1].Length(); 
	scale.z = cols[2].Length(); 

	if (GetDeterminant() < 0) scale = -scale; 

	if(scale.x) cols[0] /= scale.x; 
	if(scale.y) cols[1] /= scale.y;
	if(scale.z) cols[2] /= scale.z;


    const T epsilon = Math::Epsilon<T>();

	rotation.y  = Math::Asin(-cols[0].z);// D. Angle around oY.

	T C = Math::Cos(rotation.y);

	if(Math::Abs(C) > epsilon)
	{
		T tan_x = cols[2].z / C;// A
		T tan_y = cols[1].z / C;// B

		rotation.x = Math::Atan2(tan_y, tan_x);

		tan_x = cols[0].x / C;// E
		tan_y = cols[0].y / C;// F
		rotation.z = Math::Atan2(tan_y, tan_x);
	}
	else
	{// oY is fixed.
		rotation.x = 0;

		T tan_x =  cols[1].y;// BDF+AE => E
		T tan_y = -cols[1].x;// BDE-AF => F

		rotation.z = Math::Atan2(tan_y, tan_x);
	}
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
    return (&Rows[0][0])[index];
}
template <typename T>
constexpr T &Matrix4<T>::Flat(size_t index) {
    return (&Rows[0][0])[index];
}

template <typename T>
constexpr T Matrix4<T>::GetDeterminant() const{
    const Matrix4<T>& m = *this;

    return m[0][0]*m[1][1]*m[2][2]*m[3][3] - m[0][0]*m[1][1]*m[2][3]*m[3][2] + m[0][0]*m[1][2]*m[2][3]*m[3][1] - m[0][0]*m[1][2]*m[2][1]*m[3][3]
        + m[0][0]*m[1][3]*m[2][1]*m[3][2] - m[0][0]*m[1][3]*m[2][2]*m[3][1] - m[0][1]*m[1][2]*m[2][3]*m[3][0] + m[0][1]*m[1][2]*m[2][0]*m[3][3]
        - m[0][1]*m[1][3]*m[2][0]*m[3][2] + m[0][1]*m[1][3]*m[2][2]*m[3][0] - m[0][1]*m[1][0]*m[2][2]*m[3][3] + m[0][1]*m[1][0]*m[2][3]*m[3][2]
        + m[0][2]*m[1][3]*m[2][0]*m[3][1] - m[0][2]*m[1][3]*m[2][1]*m[3][0] + m[0][2]*m[1][0]*m[2][1]*m[3][3] - m[0][2]*m[1][0]*m[2][3]*m[3][1]
        + m[0][2]*m[1][1]*m[2][3]*m[3][0] - m[0][2]*m[1][1]*m[2][0]*m[3][3] - m[0][3]*m[1][0]*m[2][1]*m[3][2] + m[0][3]*m[1][0]*m[2][2]*m[3][1]
        - m[0][3]*m[1][1]*m[2][2]*m[3][0] + m[0][3]*m[1][1]*m[2][0]*m[3][2] - m[0][3]*m[1][2]*m[2][0]*m[3][1] + m[0][3]*m[1][2]*m[2][1]*m[3][0];
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