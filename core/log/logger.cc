#include <mozi/core/common/data_structure/double_buffered_queue.h>
#include <mozi/core/log/logger.h>
#include <mozi/core/log/ostream_consumer.h>
#include <mozi/core/log/stdout_consumer.h>
#include <mozi/core/log/stderr_consumer.h>
#include <mozi/core/log/color.h>

#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>

namespace mozi {
namespace core {
namespace log {

struct Resources
{
    DoubleBufferedQueue<Logger::Entry> logs;
    std::vector<std::unique_ptr<LogConsumer>> consumers;
    std::unique_ptr<std::thread> logging_thread;
    std::condition_variable cv;
    
    std::mutex cv_mtx;
    bool logging;
    bool work;
    int current_loop;

    std::mutex config_mtx;
    bool filenames;
    bool functions;
    std::unique_ptr<std::regex> category_filter;
    std::unique_ptr<std::regex> filename_filter;
    std::unique_ptr<std::regex> error_string_filter;

    std::atomic<Logger::Kind> verbosity;

    Resources()
        : logging(false)
        , work(false)
        , current_loop(0)
        , filenames(false)
        , functions(true)
        , verbosity(Logger::ERROR)
    {
#if STDOUTERR_LOG_CONSUMER
        consumers.emplace_back(new StdErrConsumer);
#else
        consumers.emplace_back(new StdoutConsumer);
#endif 
    }

    ~Resources()
    {
        Logger::KillThread();
    }
};

static struct Resources resources_;

void Logger::RegisterConsumer(std::unique_ptr<LogConsumer>&& consumer)
{
    std::unique_lock<std::mutex> guard(resources_.config_mtx);
    resources_.consumers.emplace_back(std::move(consumer);)
}

void Logger::ClearConsumers()
{
    std::unique_lock<std::mutex> working(resources_.cv_mtx);
    resources_.cv.wait(working,
            [&]()
            {
                return resources_.logs.BothEmpty();
            });
    std::unique_lock<std::mutex> guard(resources_.config_mtx);
    resources_.consumers.clear();
}

void Logger::Reset()
{
    std::unique_lock<std::mutex> config_guard(resources_.config_mtx);
    resources_.category_filter.reset();
    resources_.filename_filter.reset();
    resources_.error_string_filter.reset();
    resources_.filenames = false;
    resources_.functions = true;
    resources_.verbosity = Logger::ERROR;
    resources_.consumers.clear();
#if STDOUTERR_LOG_CONSUMER
    resources_.consumers.emplace_back(new StdErrConsumer);
#else
    resources_.consumers.emplace_back(new StdoutConsumer);
#endif
}

void Logger::Flush()
{
    std::unique_lock<std::mutex> guard(resources_.cv_mtx);
    if (!resources_.logging && !resources_.logging_thread)
    {
        return;
    }

    int last_loop = -1;

    for (int i = 0; i < 2; ++i)
    {
        resources_.cv.wait(guard,
                [&]()
                {
                    return !resources_.logging ||
                            (resources_.logs.Empty() &&
                             (last_loop != resources_.current_loop ||
                              resources_.logs.BothEmpty()))
                });
        last_loop = resources_.current_loop;
    }
}

void Logger::run()
{
    std::unique_lock<std::mutex> guard(resources_.cv_mtx);

    while (resources_.logging)
    {
        resources_.cv.wait(guard,
                [&]()
                {
                    return !resources_.logging || resources_.work;
                });
        
        resources_.work = false;

        guard.unlock();

        {
            resources_.logs.Swap();
            while (!resources_.logs.Empty())
            {
                std::unique_lock<std::mutex> config_guard(resources_.config_mtx);
                if (preprocess(resources_.logs.Front()))
                {
                    for (auto& consumer : resources_.consumers)
                    {
                        consumer->Consume(resources_.logs.Front());
                    }
                }
                resources_.logs.Pop();
            }
        }

        guard.lock();

        if (++resources_.current_loop > 10000)
        {
            resources_.current_loop = 0;
        }

        resources_.cv.notify_all();
    }
}

void Logger::ReportFilenames(bool report)
{
    std::unique_lock<std::mutex> config_guard(resources_.config_mtx);
    resources_.filenames = report;
}

void Logger::ReportFunctions(bool report)
{
    std::unique_lock<std::mutex> config_guard(resources_.config_mtx);
    resources_.functions = report;
}

bool Logger::preprocess(Logger::Entry& entry)
{
    if (resources_.category_filter && !regex_search(entry.context.category,
                *resources_.category_filter))
    {
        return false;
    }

    if (resources_.filename_filter && !regex_search(entry.context.filename,
                *resources_.filename_filter))
    {
        return false;
    }

    if (resources_.error_string_filter && !regex_search(entry.message,
                *resources_.error_string_filter))
    {
        return false;
    }

    if (!resources_.filenames)
    {
        entry.context.filename = nullptr;
    }

    if (!resources_.functions)
    {
        entry.context.function = nullptr;
    }

    return true;
}

void Logger::KillThread()
{
    {
        std::unique_lock<std::mutex> guard(resources_.cv_mtx);
        resources_.logging = false;
        resources_.work = false;
    }

    if (resources_.logging_thread)
    {
        resources_.cv.notify_all();
        resources_.logging_thread->join();
        resources_.logging_thread.reset();
    }
}

void Logger::QueueLog(const std::string& message,
        const Logger::Context& context,
        Logger::Kind kind)
{
    {
        std::unique_lock<std::mutex> guard(resources_.cv_mtx);
        if (!resources_.logging && !resources_.logging_thread)
        {
            resources_.logging = true;
            resources_.logging_thread.reset(new thread(Logger::run));
        }
    }

    std::string timestamp;
    getTimestamp(timestamp);
    resources_.logs.Push(Logger::Entry{message, context, kind, timestamp});
    
    {
        std::unique_lock<std::mutex> guard(resources_.cv_mtx);
        resources_.work = true;
    }

    resources_.cv.notify_all();
}

Logger::Kind Logger::GetVerbosity()
{
    return resources_.verbosity;
}

void Logger::SetVerbosity(Logger::Kind kind)
{
    std::unique_lock<std::mutex> config_guard(resources_.config_mtx);
    resources_.verbosity = kind;
}

void Logger::SetCategoryFilter(const std::regex& filter)
{
    std::unique_lock<std::mutex> config_guard(resources_.config_mtx);
    resources_.category_filter.reset(new std::regex(filter));
}

void Logger::SetFilenameFilter(const std::regex& filter)
{
    std::unique_lock<std::mutex> config_guard(resources_.config_mtx);
    resources_.filename_filter.reset(new std::regex(filter));
}

void Logger::SetErrorStringFilter(const std::regex& filter)
{
    std::unique_lock<std::mutex> config_guard(resources_.config_mtx);
    resources_.error_string_filter.reset(new std::regex(filter));
}

void Logger::getTimestamp(std::string& timestamp)
{
    std::stringstream stream;
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::chrono::system_clock::duration tp = now.time_since_epoch();
    tp -= std::chrono::duration_cast<std::chrono::seconds>(tp);
    auto ms = static_cast<unsigned>(tp / std::chrono::milliseconds(1));
    stream << std::put_time(localtime(&now_c), "%F %T") << "."
        << std::setw(3) << std::setfill('0') << ms << " ";
    timestamp = stream.str();
}

void LogConsumer::printTimestamp(std::ostream& stream,
        const Logger::Entry& entry,
        bool color) const
{
    std::string white = (color) ? C_B_WHITE : "";
    stream << white << entry.timestamp;
}

void LogConsumer::printHeader(std::ostream& stream,
        const Logger::Entry& entry,
        bool color) const
{
    std::string c_b_color = (!color) ? "" :
        (entry.kind == Logger::Kind::ERROR) ? C_B_RED :
        (entry.kind == Logger::Kind::WARNING) ? C_B_YELLOW :
        (entry.kind == Logger::Kind::INFO) ? C_B_GREEN : "";
    
    std::string white = (color) ? C_B_WHITE : "";

    std::string kind = (entry.kind == Logger::Kind::ERROR) ? "ERRPR" :
        (entry.kind == Logger::Kind::WARNING) ? "WARNING" :
        (entry.kind == Logger::Kind::INFO) ? "INFO" : "";

    stream << c_b_color << "[" << white << entry.context.category << c_b_color 
        << c_b_color << " " << kind << "] ";
}

void LogConsumer::printContext(std::ostream& stream,
        const Logger::Entry& entry,
        bool color) const
{
    if (color)
    {
        stream << C_B_BLUE;
    }

    if (entry.context.filename)
    {
        stream << " (" << entry.context.filename;
        stream << ":" << entry.context.line << ")";
    }

    if (entry.context.function)
    {
        stream << " -> Function ";
        if (color)
        {
            stream << C_CYAN;
        }
        stream << entry.context.function;
    }
}

void LogConsumer::printMessage(std::ostream& stream,
        const Logger::Entry& entry,
        bool color) const
{
    std::string white = (color) ? C_WHITE : "";
    stream << white << entry.message;
}

void LogConsumer::printNewLine(std::ostream& stream,
        bool color) const
{
    std::string def = (color) ? C_DEF : "";
    stream << def << std::endl;
}
} // namespace log
} // namespace core
} // namespace mozi
