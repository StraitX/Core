#ifndef STRAITX_AABB3_HPP
#define STRAITX_AABB3_HPP

#include "core/math/vector3.hpp"

template <typename Type>
struct AABB3 {
	Vector3<Type> Min;
	Vector3<Type> Max;

	AABB3(const Vector3<Type> &position, const Vector3<Type> &size):
		Min(position),
		Max(position + size)
	{}

	bool Inside(const Vector3<Type>& point)const{
		return Min.x <= point.x && point.x <= Max.x
			&& Min.y <= point.y && point.y <= Max.y
			&& Min.z <= point.z && point.z <= Max.z;
	}

	bool Intersects(const AABB3<Type> &other)const{
		return Min.x <= other.Max.x && Max.x >= other.Min.x
			&& Min.y <= other.Max.y && Max.y >= other.Min.y
			&& Min.z <= other.Max.z && Max.z >= other.Min.z;
	}

	Vector3<Type> Position()const {
		return Min;
	}

	Vector3<Type> Size()const {
		return Max - Min;
	}

	Vector3<Type> Center()const {
		return Min + Size()/Type(2);
	}
};

using AABB3s = AABB3<s32>;
using AABB3u = AABB3<u32>;
using AABB3f = AABB3<float>;
using AABB3d = AABB3<double>;

#endif//STRAITX_AABB3_HPP