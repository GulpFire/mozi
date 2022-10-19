#include <mozi/core/log/stdout_consumer.h>

#include <iostream>
#include <iomanip>

namespace mozi {
namespace core {
namespace log {

std::ostream& StdoutConsumer::getStream(const Logger::Entry& entry)
{
    (void) entry;
    return std::out;
}

} // namespace log
} // namespace core
} // namespace mozi
