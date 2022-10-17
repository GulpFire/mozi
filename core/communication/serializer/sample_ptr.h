#ifndef MOZI_CORE_COMMUNICATION_SERIALIZER_SAMPLE_H
#define MOZI_CORE_COMMUNICATION_SERIALIZER_SAMPLE_H

#include <memory>

namespace mozi {
namespace core {
namespace communication {

template <typename Sample>
class SamplePtr
{
	public:
	SamplePtr() noexcept
		: data_ptr_(nullptr)
		{}
	
	constexpr SamplePtr(std::nullptr) noexcept
		: SamplePtr()
		{}



	private:
	class Deleter
	{
		public:
			using DeleteFunc = std::function<void(Sample*)>;
			Deleter()
				: delete_func_([](Sample* ptr) { delete ptr; })
				{
				}

			explicit Deleter(const DeleteFunc& f)
				: delete_func_(f)
				{
				}

			void operator()(Sample* ptr) const
			{
				delete_func_(ptr);
			}

		private:
			DeleteFunc delete_func_;
	};
	

	std::unique_ptr<Sample, Deleter> data_ptr_;
	bool is_heap_allocated{true};
};

} // namespace communication
} // namespace core
} // namespace mozi

#endif
