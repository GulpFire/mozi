#ifndef MOZI_MATH_GEOMETRY_PLANE_H
#define MOZI_MATH_GEOMETRY_PLANE_H

#include "vector3.h"

template <typename T>
class Plane
{
	public:
		Vector3<T> normal_vector_;
		T second_norm_;	// length
		T squared_second_norm_;	// squared length

		inline bool isPointOnPositiveSide(const Vector3<T>& Q) const
		{
			return (n_.dotProduct(Q) + second_norm_ >= 0) ? true : false;
		}

		Plane() = default;
		explicit Plane(const Vector3<T>& normal_vector, 
				const Vector3<T>& point)
			: normal_vector_(normal_vector)
			, second_norm_(-normal_vector_.dotProduct(point))
			, squared_second_norm_(normal_vector_.x * normal_vector_.x 
					+ normal_vector_.y * normal_vector_.y
					+ normal_vector_.z * normal_vector_.z)
		{
			
		}
};

#endif
