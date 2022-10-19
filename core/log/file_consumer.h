#ifndef MOZI_CORE_LOG_FILE_CONSUMER_H
#define MOZI_CORE_LOG_FILE_CONSUMER_H

#include <mozi/core/log/logger.h>
#include <mozi/core/log/ostream_consumer.h>

#include <fstream>

namespace mozi {
namespace core {
namespace log {

class FileConsumer
{
    public:
        FileConsumer();
        FileConsumer(const std::string& filename,
                bool append = false);
        virtual ~FileConsumer();
        
    private:
        virtual std::ostream& getStream(const Logger::Entry& entry) override;

        std::string output_file_;
        std::ostream file_;
        bool append_;
};

} // namespace log
} // namespace core
} // namespace mozi

#endif
