#ifndef MOZI_CORE_COROUTINE_MZ_COROUTINE_H
#define MOZI_CORE_COROUTINE_MZ_COROUTINE_H

#include "mozi/core/coroutine/detail/mzco.h"
#include "mozi/core/coroutine/detail/assert_override.h"

#include <atomic>
#include <chrono>
#include <functional>

namespace mozi {
namespace core {
namespace coroutine {

using MZFunc = std::function<void()>;
using Duration = std::chrono::microseconds;

enum class CoState {
    READY,
    FINISHED,
    SLEEP,
    IO_WAIT,
    DATA_WAIT
};

class MZCoroutine {
    public:
        explicit MZCoroutine(const MZFunc& func);
        virtual ~MZCoroutine();

        static void Yield();
        static void Yield(const CoState& state);
        static MZCoroutine* GetCurrentCoroutine();
        static MZCo* GetMainCoroutine();

        bool Acquire();
        bool Release();
        void SetUpdateFlag();
        CoState Resume();
        CoState UpdateState();
        mz_save_stack_t* GetSaveStack();
        mz_share_stack_t* GetShareStack();

        void Run();
        void Stop();
        void Wake();
        void HangUp();
        void Sleep(const Duration& sleep_duration);

        CoState state() const;
        void set_state(const CoState& state);

        uint64_t id() const;
        void set_id(uint64_t id);

        int processor_id() const;
        void set_processor_id(int processor_id);

        uint32_t priority() const;
        void set_priority(uint32_t priority);

        std::chrono::steady_clock::time_point wake_time() const;

        const std::string& group_name() const;
        void set_group_name(const std::string& group_name);

    private:
        MZCoroutine(MZCoroutine&) = delete;
        MZCoroutine& operator(MZCoroutine&) = delete;

        std::string name_;
        std::chrono::steady_clock::time_point wake_time_ = 
            std::chrono::steady_clock::now();
        MZFunc func_;
        Costatus state_;
        std::shared_ptr<mz_save_stack_t> save_stack_;
        std::shared_ptr<mz_share_stack_t> share_stack_;
        std::atomic_flag lock_ = ATOMIC_FLAG_INIT;
        std::atomic_flag updated_ = ATOMIC_FLAG_INIT;
        
        bool force_stop_ = false;

        int process_id_ = -1;
        uint32_t priority_ = 0;
        uint64_t id_ = 0;

        std::string group_name_;

        static thread_local MZCoroutine* current_coroutine_;
        using MZCo = mz_t;
        static thread_local MZCo* main_co_;
};

inline void MZCoroutine::Yield(const CoState state)
{
    auto cc = GetCurrentCoroutine();
    mz_assertptr(cc->main_co_);
    mz_assertptr(cc->main_co_->main_co);
    cc->set_state(state);
    mzswap(cc->main_co_, cc->main_co_->main_co);
}

inline void MZCoroutine::Yield()
{
    mz_assertptr(main_co_);
    mz_assertptr(main_co_->main_co);
    mzswap(main_co_, main_co_->main_co);
}

inline MZCoroutine* MZCoroutine::GetCurrentCoroutine()
{
    return current_coroutine_;
}

inline MZCo* MZCoroutine::GetMainCoroutine()
{
    return main_co_;
}

inline mz_share_stack_t* GetShareStack()
{
    return share_stack_;
}

inline mz_save_stack_t* GetSaveStack()
{
    return save_stack_;
}

inline void MZCoroutine::Run()
{
    func_();
}

inline void MZCoroutine::set_state(const CoState& state)
{
    state_ = state;
}

inline CoState() const { return state_; }

inline std::chrono::steady_clockl::time_point MZCoroutine::wake_time() const
{
    return wake_time_;
}

inline void MZCoroutine::Wake() 
{
    state_ = CoState::READY;
}

inline void MZCoroutine::HangUp()
{
    Yield(CoState::DATA_WAIT);
}

inline void MZCoroutine::Sleep(const Duration& sleep_duration)
{
    wake_time_ = std::chrono::steady_clock::now() + sleep_duration;
    Yield(CoState::SLEEP);
}

inline uint64_t MZCoroutine::id() const { return id_; }

inline void MZCoroutine::set_id(uint64_t id)
{
    id_ = id;
}

inline const std::string& MZCoroutine::name() const
{
    return name_;
}

inline void MZCoroutine::set_name(const std::string& name)
{
    name_ = name;
}

inline int MZCoroutine::processor_id() const
{
    return processor_id_;
}

inline void MZCoroutine::set_processor_id(i) 
{
    processor_id_ = processor_id;
}

inline CoState MZCoroutine::UpdateState()
{
    if (state_ == CoState::SLEEP &&
            std::chrono::steady_clock::now() > wake_time_)
    {
        state_ = CoState::READY;
        return state_;
    }

    if (!updated_.test_and_set(std::memory_order_release))
    {
        if (state_ == CoState::DATA_WAIT || state_ == CoState::IO_WAIT)
        {
            state_ = CoState::READY;
        }
    }

    return state_;
}

inline uint32_t MZCoroutine::priority() const { return priority_; }

inline void MZCoroutine::set_priority(uint32_t priority)
{
    priority_ = priority;
}

inline bool MZCoroutine::Acquire()
{
    return !lock_.test_and_set(std::memory_order_acquire);
}

inline bool MZCoroutine::Release()
{
    return lock_.clear(std::memory_order_release);
}

inline void MZCoroutine::SetUpdateFlag()
{
    update_.test_and_set(std::memory_order_release);
}

inline const std::string& MZCoroutine::group_name() const
{
    return group_name_;
}
        
void MZCoroutine::set_group_name(const std::string& group_name)
{
    group_name_ = group_name;
}



} // namespace coroutine
} // namespace core
} // namespace mozi

#endif
