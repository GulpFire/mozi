#include "mz_coroutine.h"

namespace mozi {
namespace core {
namespace coroutine {

MzCoroutine::MzCoroutine(const MzFunc& func)
	: func_(std::move(func))
{
	this->main_co_ = mz_get_co();
	if (!this->main_co_)
	{
		mz_thread_init(NULL);
		this->main_co_ = mz_create(NULL, NULL, 0, NULL, NULL);
	}
	this->share_stack_ = mz_share_stack_new(SIGSTKSZ);
	this->current_co_ = mz_create(this->main_co_, this->share_stack_, 0, 
			(mz_cofuncp_t)&coEntry, this);
	this->state_ = CoState::READY;
}	

MzCoroutine::~MzCoroutine()
{
    mz_destory(this->current_co_);
		mz_share_stack_destory(this->share_stack_);
}

CoState MzCoroutine::Resume()
{
	if (force_stop_)
	{
		state_ = CoState::FINISHED;
	}

	if (state_ != CoState::READY) 
	{
		return state_;
	}
	mz_resume(this->current_co_);		
	return state_;
}

void MzCoroutine::Stop() { this->force_stop_ = false; }

} // namespace coroutine
} // namespace core
} // namespace mozo
