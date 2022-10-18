#ifndef MOZI_CORE_LOG_LOGGER_H
#define MOZI_CORE_LOG_LOGGER_H

#include "glog/logging.h"

#include <mutex>

namespace mozi {
namespace core {
namespace logger {

class Logger : public google::base::Logger
{
    public:
        explicit Logger(google::base::Logger* wrapped);
        ~Logger();
        void Write(bool force_flush, time_t timestamp, const char* message,
                int message_len) override;
        void Flush() ove rride;
        uint32_t LogSize() override;
    private:
        google::base::Logger* const wrapped_;
        std::mutex mutex_;
};

} // namespace logger
} // namespace core
} // namespace mozi
#endif
