#ifndef MOZI_MATH_GEOMETRY_BASE_RAY_H
#define MOZI_MATH_GEOMETRY_BASE_RAY_H

namespace mozi {
namespace math {

template <typename T>
struct Ray
{
	const Vector3<T> start_point_;
	const Vector3<T> another_point_;
	const T inv_length_squared_;

	Ray(const Vector3<T>& start_point, const Vector3<T>& another_point)
		: start_point_(start_point)
		, another_point_(another_point)
	{}
};

} // namespace math
} // namespace mozi

#endif
