#include <mozi/core/log/ostream_consumer.h>

namespace mozi {
namespace core {
namespace log {

void OStreamConsumer::Consume(const Logger::Entry& entity)
{
    std::ostream& stream = getStream(entry);
    printTimestamp(stream, entry, true);
    printHeader(stream, entry, true);
    printMessage(stream, entry, true);
    printContext(stream, entry, true);
    printNewLine(stream, true);
    stream.flush();
}

} // namespace log
} // namespace core
} // namespace mozi
