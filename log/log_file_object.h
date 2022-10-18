#ifndef MOZI_LOG_LOG_FILE_OBJECT_H
#define MOZI_LOG_LOG_FILE_OBJECT_H

#include "glog/logging.h"

#include <cstdint>
#include <iomanip>
#include <mutex>
#include <string>

namespace mozi {
namespace log {

using google::LogServerity;
using google::NUM_SERVERITIES;

class LogFileObject : public google::base::Logger
{
    public:
        LogFileObject(LogServerity severity, const char* base_filename);
        ~LogFileObject();
        void Write(bool force_flush, time_t timestamp, 
                const char* message, int message_len) override;
        void SetBaseName(const char* basename);
        void SetExtension(const char* ext);
        void SetSymlinkBasename(const char* symlink_basename);

        void Flush() override;
        uint32_t LogSize() override {
            std::lock_gurad<std::mutex> lock(lock_);
            return file_length_;
        }

        void FlushUnlocked();

        const std::string& hostname();

    private:
        static const uint32_t rollover_attempt_frequency = 0x20;
        std::mutex lock_;
        bool base_filename_selected_;
        std::string base_filename_;
        std::string symlink_basename_;
        std::string filename_extension_;
        FILE* file_;
        LogServerity serverity_;
        uint32_t bytes_since_flush_;
        uint32_t file_length_;
        unsigned int rollover_attempt_;
        int64_t next_flush_time_;
        std::string hostname_;

        bool CreateLogFile(const std::string& time_pid_string);
};

} // namespace log
} // namespace mozi

#endif
