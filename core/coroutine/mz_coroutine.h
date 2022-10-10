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
        static mz_t* GetMainStack();

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
        static thread_local std::unique_ptr<mz_t> main_stack_;
};

} // namespace coroutine
} // namespace core
} // namespace mozi

#endif
