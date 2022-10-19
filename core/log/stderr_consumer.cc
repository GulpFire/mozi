#include <mozi/core/log/stderr_consumer.h>

#include <iomanip>

void StdErrConsumer::StdErrThreshold(const Logger::Kind& kind)
{
    stderr_threshold_ = kind;
}

Logger::Kind StdErrThreshold() const
{
    return stderr_threshold_;
}

std::ostream& StdErrConsumer::getStream(const Logger::Entry& entry)
{
    if (entry.kind <= stderr_threshold_)
    {
        return std::cerr;
    }
    return std::cout;
}
