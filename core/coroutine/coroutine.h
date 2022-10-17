#ifndef COROUTINE_H
#define COROUTINE_H

#include "detail/mzco.h"
#include <csignal>
#include <functional>
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

class Coroutine
{
    public:

using MzFunc = std::function<void (Coroutine&)>;
        explicit Coroutine(MzFunc f)
            : func_(std::move(f))
        {
            this->main_co_ = mz_get_co();
            if (!this->main_co_)
            {
                mz_thread_init(NULL);
                this->main_co_ = mz_create(NULL, NULL, 0, NULL, NULL);
            }
            this->share_stack_ = mz_share_stack_new(SIGSTKSZ);
            this->current_co_ = mz_create(this->main_co_, 
                    this->share_stack_, 0, (mz_cofuncp_t)&fEntry,  this);
        }

        ~Coroutine() noexcept
        {
            mz_destory(this->current_co_);
            mz_share_stack_destory(this->share_stack_);
        }

        void yield()
        {
            mz_yield();
        }

        void resume()
        {
            mz_resume(this->current_co_);
        }
    private:
        Coroutine(const Coroutine&) = delete;
        Coroutine& operator = (const Coroutine&) = delete;
        CoState state_;
        MzFunc func_;
        mz_share_stack_t* share_stack_;
        mz_t* main_co_;
        mz_t* current_co_;

        static void fEntry()         
        {
            auto* coroutine = static_cast<Coroutine*>(mz_get_arg());
            coroutine->func_(*coroutine);
        }

};
}
}
}

#endif
