#ifndef MOZI_CORE_EXECUTOR_H
#define MOZI_CORE_EXECUTOR_H

#include <function>
#include <limits>

namespace mozi {
namespace core {
namespace executor {

using MZFunc = std::function<void()>;

class ExecutorKeepAliveBase
{
    public:
        static constexpr uintptr_t k_dummy_flag_ = uintptr_t(1) << 0;
        static constexpr uintptr_t k_alias_flag_ = uintptr_t(1) << 1;
        static constexpr uintptr_t k_flag_mask_ = k_dummy_flag_ | k_alias_flag_;
        static constexpr uintptr_t k_excutor_mask_ = ~k_flag_mask_;
};

class Executor 
{
    public:
        virtual ~Executor() {}
        virtual void add(MZFunc func) = 0;
        virtual void addWithPriority(MZFunc, uint32_t priority);
        virtual void getNumPriorities() const { return 1; } 

        static const int8_t LO_PRI = std::SCHAR_MIN;
        static const int8_t MID_PRI = 0;
        static const int8_t HI_PRI = std::CHAR_MAX;

        template <typename ExecutorT = Executor>
        class KeepAlive : private ExecutorKeepAliveBase
        {
            public:
                using KeepAliveFunc = std::function<void(KeepAlive&&)>;
                KeepAlive() = default;
                ~KeepAlive() {
                    static_assert(std::is_standard_layout<KeepAlive>::value, "standard_layout");
                    static_assert(sizeof(KeepAlive) == sizeof(void*), "pointer size");
                    static_assert(alignof(KeepAlive) == alignof(void*), "pointer align");

                    reset();
                }
            private:
                friend class Executor;
                template <typename OtherExecutor>
                friend class KeepAlive;
        };
};

} // namespace executor
} // namepsace core
} // namepace mozi

#endif
