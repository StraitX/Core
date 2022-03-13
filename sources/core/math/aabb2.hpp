#ifndef STRAITX_AABB2_HPP
#define STRAITX_AABB2_HPP

#include "core/math/vector2.hpp"

template <typename Type>
struct AABB2 {
	Vector2<Type> Min;
	Vector2<Type> Max;

	AABB2(const Vector2<Type> &position, const Vector2<Type> &size):
		Min(position),
		Max(position + size)
	{}

	bool Inside(const Vector2<Type>& point)const{
		return Min.x <= point.x && point.x <= Max.x
			&& Min.y <= point.y && point.y <= Max.y;
	}

	bool Intersects(const AABB2<Type> &other)const{
		return Min.x <= other.Max.x && Max.x >= other.Min.x
			&& Min.y <= other.Max.y && Max.y >= other.Min.y;
	}

	Vector2<Type> Position()const {
		return Min;
	}

	Vector2<Type> Size()const {
		return Max - Min;
	}

	Vector2<Type> Center()const {
		return Min + Size()/Type(2);
	}
};

using AABB2s = AABB2<s32>;
using AABB2u = AABB2<u32>;
using AABB2f = AABB2<float>;
using AABB2d = AABB2<double>;

#endif//STRAITX_AABB2_HPP