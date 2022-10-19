#include <mozi/core/log/file_consumer.h>

#include <iomanip>

namespace mozi {
namespace core {
namespace log {

FileConsumer::FileConsumer()
    : FileConsumer("output.log")
{
    
}

FileConsumer::FileConsumer(const std::string& filename, bool append)
    : output_file_(filename)
    , append_(append)
{
    if (append_)
    {
        file_.open(output_file_, std::ios::out | std::ios::app);
    }
    else 
    {
        file_.open(output_file_, std::ios::out);
    }
}

FileConsumer::~FileConsumer()
{
    file_.close();
}

std::ostream& FileConsumer::getStream(const Logger::Entry& entry)
{
    (void) entry;
    return file_;
}

} // namespace log
} // namespace core
} // namespace mozi
