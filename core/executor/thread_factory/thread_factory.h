#ifndef MOZI_CORE_EXECUTOR_THREAD_FACTORY_THREAD_FACTORY_H
#define MOZI_CORE_EXECUTOR_THREAD_FACTORY_THREAD_FACTORY_H

#include "executor.h"

#include <thread>

namespace mozi {
namespace core {
namespace executor
{

class ThreadFactory
{
public:
    virtual ~ThreadFactory() = default;
    virtual std::thread NewThread(MZFunc& func) = 0;
};

} // executor
} // core
} // mozi

#endif
