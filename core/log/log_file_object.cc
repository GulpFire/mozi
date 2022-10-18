#include "log_file_object.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cassert>
#include <iomanip>
#include <iostream>
#include <vector>

#include "log_util.h"

namespace mozi {
namespace core {
namespace log {

#define PATH_SEPARATOR '/'

static bool stop_writing = false;

const char* const LogServerityNames[NUM_SERVERITIES] = {"INFO", "WARNING", "ERROR", "FATAL"};

LogFileObject::LogFileObject(LogServerity serverity, const char* base_filename)
    : base_filename_selected_(base_filename != nullptr)
    , base_filename_((base_filename != nullptr) ? base_filename : "")
    , symlink_basename_("UNKNOWN")
    , file_extension_()
    , file_(NULL)
    , severity_(severity)
    , bytes_since_flush_(0)
    , file_length_(0)
    , rollover_attempt_(rollover_attempt_frequency_ - 1)
    , next_flush_time_(0)
{
    if (base_filename_.empty())
    {
        base_filename_ = "UNKNOWN";
    }

    assert(severity >= 0);
    assert(severity < NUM_SEVERITIES);
}

LogFileObject::~LogFileObject()
{
    std::lock_gurad<std::mutex> lock(lock_);
    if (file_ != nullptr)
    {
        fclose(file_);
        flie_ = nullptr;
    }
}



} // namespace log
} // namespace core
} // namespace mozi
