#ifndef MOZI_CORE_UTILS_TIMED_MUTEX_H
#define MOZI_CORE_UTILS_TIMED_MUTEX_H

#include <mutex>

namespace mozi {
namespace core {

using TimedMutex = std::timed_mutex;
using RecursiveTimeMutex = std::recursive_timed_mutex;

} // namespace core
} // namespace mozi

#endif
