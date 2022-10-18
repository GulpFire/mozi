#ifndef MOZI_MATH_GEOMETRY_BASE_POOL_H
#define MOZI_MATH_GEOMETRY_BASE_POOL_H

#include <memory>
#include <vector>

namespace mozi {
namespace math {

template <typename S>
class Pool
{
	public:
		inline void clear()
		{
			data.clear();
		}

		inline void reclaim(std::unique_ptr<S>& ptr)
		{
			data_.push_back(std::move(ptr));
		}

		inline std::unique_ptr<S> get()
		{
			if (data_.empty())
			{
				return std::unique_ptr<S>(new S());
			}
			auto it = data_.end() - 1;
			std::unique_ptr<S> r = std::move(*it);
			data_.erase(it);
			return r;
		}

	private:
		std::vector<>std::unique_ptr<S> data_;
};

} // namespace math
} // namespace mozi

#endif
