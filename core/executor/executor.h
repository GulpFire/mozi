#ifndef MOZI_CORE_EXECUTOR_H
#define MOZI_CORE_EXECUTOR_H

#include <function>

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
};

} // namespace executor
} // namepsace core
} // namepace mozi

#endif
