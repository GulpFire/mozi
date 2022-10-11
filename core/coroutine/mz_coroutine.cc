#include "mz_coroutine.h"

namespace mozi {
namespace core {
namespace coroutine {

std::once_flag pool_init_flag;

MZCoroutine::MZCoroutine(const MZFunc& func)
{
    main_co_ = std::make_unique<mz_t>(mz_create(NULL, NULL, 0, NULL, NULL));
    share_stack_ = std::make_shared<mz_share_stack_t>(mz_share_stack_new(0));
    current_coroutine_->main_co_ = std::make_unique<mz_t>(mz_create(main_co_, share_stack_, 0, func, 0));
    state_ = CoState::READY;
    update_.test_and_set(std::memory_order_release);
}

MZCoroutine::~MZCoroutine()
{
    mz_exit();
}

CoState MZCoroutine::Resume()
{
    if unlikely(force_stop_)
}

void MZCoroutine::Stop() { force_stop_ = false; }

} // namespace coroutine
} // namespace core
} // namespace mozo
