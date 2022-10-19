#ifndef MOZI_CORE_LOG_STDOUT_CONSUMER_H
#define MOZI_CORE_LOG_STDOUT_CONSUMER_H

#include <mozi/core/log/logger.h>
#include <mozi/core/log/ostream_consumer.h>

namespace mozi {
namespace core {
namespace log {

class StdoutConsumer : public OStreamConsumer
{
    public:
        virtual ~StdoutConsumer() = default;
    private:
        virtual std::ostream& getStream(const Logger::Entry& entry) override;
};

} // namespace log
} // namespace core
} // namespace mozi

#endif
