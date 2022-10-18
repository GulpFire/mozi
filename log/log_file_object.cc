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

void LogFileObject::SetBasename(const char* basename)
{
    std::lock_guard<std::mutex> lock(lock_);
    base_file_selected_ = true;
    if (base_filename_ != basename)
    {
        if (file_ != nullptr)
        {
            fclose(file_);
            file_ = nullptr;
            rollover_attempt_ = rollover_attempt_frequency - 1;
        }
        base_filename_ = basename;
    }
}

void LogFileObject::SetExtension(const char* ext)
{
    std::lock_guard<std::mutex> lock(lock_);
    if (filename_extension_ != ext)
    {
        if (file_ != nullptr)
        {
            fclose(file_);
            file_ = nullptr;
            rolover_attempt_ = rollover_attempt_frequency_ - 1;
        }
        filename_extension_ = ext;
    }
}

void LogFileObject::SetSymlinkBasename(const char* symlink_basename)
{
    std::lock_guard<std::mutex> lock(lock_);
    symlink_basename_ = symlink_basename;
}

void LogFileObject::Flush()
{
    std::lock_guard<std::mutex> lock(lock_);
    FlushUnlocked();
}

void LogFileObject::FlushUnlocked()
{
    if (file_ != nullptr)
    {
        fflush(file_);
        bytes_since_flush_ = 0;
    }
    
    const int64_t next = 
        (FLAGS_logbufsecs * static_cast<int64_t>(1000000));
    next_flush_time_ = CycleClock_Now() + UsecToCycles(next);
}

bool LogFileObject::CreateLogfile(const std::string& time_pid_string)
{
    std::string string_filename = 
        base_filename_ + filename_extension_ + time_pid_string;
    const char* filename = string_filename.c_str();
    int fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, FLAGS_logfile_mode);
    if (fd == -1)
    {
        return false;
    }

    fcntl(fd, F_SETFD, FD_CLOEXEC);

    file_ = fdopen(fd, "a");
    if (file == nullptr)
    {
        close(fd);
        unlink(filename);
        return false;
    }

    if (!symlink_basename_.empty())
    {
        const char* slash = strrchr(filename, PATH_SEPARATOR);
        const std::string linkname = 
            symlink_basename_ + '.' + LogSeverityNames[severity_];
        std::string linkpath;
        if (slash)
        {
            linkpath = std::string(filename, slash - filename + 1);
        }
        linkpath += linkname;
        unlink(linkpath.c_str());

        const char* linkdest = slash ? (slash + 1) : filename;
        if (symlink(linkdest, linkpath.c_str()) != 0)
        {
            MZINFO << "symlink failed" ;
        }

        if (!FLAGS_log_link.empty())
        {
            linkpath = FLAGS_log_link + "/" + linkname;
            unlink(linkpath.c_str());
            if (symlink(filename, linkpath.c_str()) != 0)
            {}
        }
    }

    return true;
}

void LogFileObject::Write(bool force_flush, time_t timestamp,
        const char* message, int message_len)
{
    std::lock_guard<std::mutex> lock(lock_);
    if ()
}

} // namespace log
} // namespace mozi
