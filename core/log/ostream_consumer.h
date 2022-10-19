#ifndef MOZI_CORE_LOG_OSTREAM_CONSUMER_H
#define MOZI_CORE_LOG_OSTREAM_CONSUMER_H

#include <mozi/core/log/logger.h>

#include <iostream>

namespace mozi {
namespace core {
namespace log {

class OStreamConsumer : public LogConsumer
{
    public:
        ~OStreamConsumer() = default;
        void Consume(const Log::Entry& entry) override;

    protected:
        virtual std::ostream& getStream(const Logger::Entry& entry) = 0;
};

} // namespace log
} // namespace core
} // namespace mozi

#endif
