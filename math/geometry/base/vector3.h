#ifndef MOZI_CORE_MATH_GEOMETRY_BASE_VECTOR3_H
#define MOZI_CORE_MATH_GEOMETRY_BASE_VECTOR3_H

namespace mozi {
namespace math {
namespace base {

template <typename T>
class Vector3
{
	public:
		Vector3() = default;
		Vector3(const T& x, const T& y, const T& z)
			: x(x), y(y), z(z)
		{}

		T x, y, z;

		inline T dotProduct(const Vector3& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}

		inline Vector3 operator-(const Vector3& other) const
		{
			return Vector3(x - other.x, y - other.y, z - other.z);
		}

		inline Vector3 operator+(const Vector3& other) const
		{
			return Vector3(x + other.x, y + other.y, z + other.z);
		}

		inline Vector3& operator+=(const Vector3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		inline Vector3& operator-=(const Vector3& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		inline Vector3& operator*=(const T& c)
		{
			x *= c;
			y *= c;
			z *= c;
			return *this;
		}

		inline Vector3& operator/=(const T& c)
		{
			x /= c;
			y /= c;
			z /= c;
			return *this;
		}

		inline Vector3 operator-() const
		{
			return Vector3(-x, -y, -z);
		}

		template <typename S>
		inline Vector3 operator*(const S& c) const
		{
			return Vector3(x * c, y * c, z * c);
		}

		template <typename S>
		inline Vector3 operator/(const S& c) const
		{
			return Vector3(x / c, y / c, z / c);
		}

		inline T getLengthSquared() const
		{
			return x * x + y * y + z * z;
		}

		inline bool operator!=(const Vector3& o) const
		{
			return x != o.x || y != o.y || z != o.z;
		}

		inline T getSquaredDistanceTo(const Vector3& other) const
		{
			const T dx = x - other.x;
			const T dy = y - other.y;
			const T dz = z - other.z;
			return dx * dx + dy * dy + dz * dz;
		}
};

template <typename T>
inline std::ostream& operaor<<(std::ostream& os, const Vector3<T>& vec)
{
	os << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
	return os;
}

template <typename T>
inline Vector3<T> operator*(T c, const Vector3<T>& v)
{
	return Vector3<T>(v.x * c, v.y * c, v.z * c);
}

} // namespace base
} // namespace math
} // namespace mozi

#endif
