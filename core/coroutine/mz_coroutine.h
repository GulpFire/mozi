#ifndef MOZI_CORE_COROUTINE_MZ_COROUTINE_H
#define MOZI_CORE_COROUTINE_MZ_COROUTINE_H

#include "detail/mzco.h"

#include <atomic>
#include <cstdint>
#include <csignal>
#include <chrono>
#include <functional>
#include <thread>
#include <memory>
#include <mutex>
#include <iostream>


namespace mozi {
namespace core {
namespace coroutine {

enum CoState
{
    READY,
    FINISHED,
    SLEEP,
    IO_WAIT,
    DATA_WAIT
};

class MzCoroutine
{
    public:
				using Duration = std::chrono::microseconds;
				using MzFunc = std::function<void (MzCoroutine&)>;
        explicit MzCoroutine(const MzFunc& func);

				virtual ~MzCoroutine();

				void Yield();

				void Yield(const CoState& state);

				mz_t* GetCurrentCoroutine();

				mz_t* GetMainCoroutine();

				bool Acquire();
				void Release();
				void SetUpdateFlag();
				CoState Resume();
				CoState UpdateState();
				mz_share_stack_t* GetShareStack();

				void Run();
				void Stop();
				void Wake();
				void HangUp();
				void Sleep(const Duration& sleep_duration);

				const std::string& name() const;
				void set_name(const std::string& name);

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
        MzCoroutine(const MzCoroutine&) = delete;
        MzCoroutine& operator = (const MzCoroutine&) = delete;
        CoState state_;
        MzFunc func_;
        mz_share_stack_t* share_stack_;
        mz_t* main_co_;
        mz_t* current_co_;

				std::string name_;
				std::chrono::steady_clock::time_point wake_time_ =
					std::chrono::steady_clock::now();
				std::atomic_flag lock_ = ATOMIC_FLAG_INIT;
				std::atomic_flag updated_ = ATOMIC_FLAG_INIT;

				bool force_stop_ = false;

				int processor_id_ = 1;
				uint32_t priority_ = 0;
				uint64_t id_ = 0;

				std::string group_name_;

        static void coEntry()         
        {
            auto* coroutine = static_cast<MzCoroutine*>(mz_get_arg());
            coroutine->Run();
						//coroutine->Yield(CoState::FINISHED);
        }

};

inline void MzCoroutine::Yield()
{
	mz_yield();
}

inline void MzCoroutine::Yield(const CoState& state)
{
	set_state(state);
	mz_yield();
}

inline mz_t* MzCoroutine::GetCurrentCoroutine() 
{
	return current_co_;
}

inline mz_t* MzCoroutine::GetMainCoroutine() 
{
	return main_co_;
}

inline mz_share_stack_t* MzCoroutine::GetShareStack()
{
	return share_stack_;
}

inline void MzCoroutine::Run()
{
	func_(*this);
}

inline void MzCoroutine::set_state(const CoState& state)
{
	state_ = state;
}

inline CoState MzCoroutine::state() const { return state_; }

inline std::chrono::steady_clock::time_point MzCoroutine::wake_time() const
{
	return wake_time_;
}

inline void MzCoroutine::Wake()
{
	set_state(CoState::READY);
}

inline void MzCoroutine::HangUp()
{
	Yield(CoState::DATA_WAIT);
}

inline void MzCoroutine::Sleep(const Duration& sleep_duration)
{
	wake_time_ = std::chrono::steady_clock::now() + sleep_duration;
	Yield(CoState::SLEEP);
}

inline uint64_t MzCoroutine::id() const { return id_; }

inline void MzCoroutine::set_id(uint64_t id)
{
	id_ = id;
}

inline const std::string& MzCoroutine::name() const
{
	return name_;
}

inline void MzCoroutine::set_name(const std::string& name)
{
	name_ = name;
}

inline int MzCoroutine::processor_id() const
{
	return processor_id_;
}

inline CoState MzCoroutine::UpdateState()
{
	if (state_ == CoState::SLEEP &&
			std::chrono::steady_clock::now() > wake_time_)
	{
		set_state(CoState::READY);
		return state_;
	}

	if (!updated_.test_and_set(std::memory_order_release))
	{
		if (state_ == CoState::DATA_WAIT || state_ == CoState::IO_WAIT)
		{
			set_state(CoState::READY);

		}
	}
	return state_;
}

inline uint32_t MzCoroutine::priority() const
{
	return priority_;
}

inline void MzCoroutine::set_priority(uint32_t priority)
{
	priority_ = priority;
}

inline bool MzCoroutine::Acquire()
{
	return !this->lock_.test_and_set(std::memory_order_acquire);
}

inline void MzCoroutine::Release()
{
	return lock_.clear(std::memory_order_release);
}

inline void MzCoroutine::SetUpdateFlag()
{
	updated_.test_and_set(std::memory_order_release);
}

inline const std::string& MzCoroutine::group_name() const
{
	return group_name_;
}

inline void MzCoroutine::set_group_name(const std::string& group_name)
{
	group_name_ = group_name;
}

}
}
}

#endif
