#ifndef MOZI_CORE_LOG_LOG_UTILS_H
#define MOZI_CORE_LOG_LOG_UTILS_H

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <unistd.h>

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

#include "mozi/core/common/global_data.h"

namespace mozi {
namespace core {
namespace log {

inline int64_t CycleClock_Now()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return static_cast<int64_t>(tv.tv_sec) * 1000000 + tv.tv_usec;
}

inline int64_t UsecToCycles(int64_t usec) { return usec; }

static inline void GetHostName(std::string* hostname)
{
    struct utsname buf;
    if (0 != uname(&buf))
    {
        *buf.nodename = '\0';
    }
    *hostname = buf.nodename;
}

int32_t GetMainThreadPid();

bool PidHasChanged();

inline int32_t MaxLogSize()
{
    return (FLAG_max_log_size > 0 ? FLAG_max_log_size : 1);
}

inline void FindModuleName(std::string* log_message, std::string module_name)
{
    auto lpos = log_message->find(LEFT_BRACKET);
    if (lpos != std::string::npos)
    {
        auto rpos = log_message->find(RIGHT_BRACKET, lpos);
        if (rpos != std::string::npos)
        {
            module_name->assign(*log_message, lpos + 1, rpos - lpos - 1);
            auto cut_length = rpos - lpos + 1;
            log_message->erase(lpos, cut_length);
        }
    }

    if (module_name->empty())
    {
        if (!common::GlobalData::Instance())
        {
            *module_name = common::Global::Data::Instance()->ProcessGroup();
        }
    }
}

} // namespace log
} // namespace core
} // namespace mozi

#endif
