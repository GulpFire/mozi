#ifndef MOZI_CORE_LOG_STDERR_CONSUMER_H
#define MOZI_CORE_LOG_STDERR_CONSUMER_H

#include <mozi/core/log/logger.h>
#include <mozi/core/log/ostream_consumer.h>

#include <iostream>

namespace mozi {
namespace core {
namespace log {

class StdErrConsumer : public OStreamConsumer
{
    public:
        virtual ~StdErrConsumer() = default;
        virtual void StdErrThreshold(const Logger::Kind& kind);
        virtual Logger::Kind StdErrThreshold() const;
        static constexpr Logger::Kind STDERR_THRESHOLD_DEFAULT = Logger::Kind::WARNING;

    protected:
        virtual std::ostream& getStream(
                const Logger::Entry& entry) override;
    private:
        Logger::Kind stderr_threshold_ = STDERR_THRESHOLD_DEFAULT;
};

} // namespace log
} // namespace core
} // namespace mozi

#endif
