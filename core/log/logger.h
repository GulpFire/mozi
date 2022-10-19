#ifndef MOZI_CORE_LOG_LOGGER_H
#define MOZI_CORE_LOG_LOGGER_H

#include <atomic>
#include <thread>
#include <sstream>
#include <regex>

#define logInfo(cat, msg) _logInfo(cat, msg)
#define logWarning(cat, msg) _logWarning(cat, msg)
#define logError(cat, msg) _logError(cat, msg)

namespace mozi {
namespace core {
namespace log {

class LogConsumer;

class Logger
{
    public:
        enum Kind
        {
            ERROR,
            WARNING,
            INFO
        };

        static void RegisterConsumer(std::unique_ptr<LogConsumer>&& consumer);
        static void ClearConsumers();
        static void ReportFileNames(bool report);
        static void ReportFunctions(bool report);
        static void SetVerbosity(Logger::Kind);
        static void Logger::Kind GetVerbosity();
        static void SetCategoryFilter(const std::regex& rgx);
        static void SetFilenameFilter(const std::regex& rgx);
        static void SetErrorStringFilter(const std::regex& rgx);
        static void Reset();
        static void Flush();
        static void KillThread();

        struct Context
        {
            const char* filename;
            int line;
            const char* function;
            const char* category;
        };

        struct Entry
        {
            std::string message;
            Logger::Context context;
            Logger::Kind kind;
            std::string timestamp;
        };

        static void QueueLog(const std::string& message,
                const Logger::Context& context, Logger::Kind kind);
    private:
        static bool preprocess(Entry& entry);
        static void run();
        static void getTimestamp(std::string& time);

};

class LogConsumer
{
    public:
        virtual ~LogConsumer() = default;

        virtual void Consume(const Logger::Entry& entry) = 0;
    
    protected:
        void printTimestamp(std::ostream& stream,
                const Logger::Entry& entry,
                bool color) const;
        
        void printHeader(std::ostream& stream,
                const Logger::Entry& entry,
                bool color) const;
        
        void printContext(std::ostream& stream,
                const Logger::Entry& entry,
                bool color) const;

        void printMessage(std::ostream& stream,
                const Logger::Entry& entry,
                bool color) const;

        void printNewLine(std::ostream& stream,
                bool color) const;
};

#define _logError(cat, msg)             \
{                                       \
    using namespace mozi::core;    \
    std::stringstream ss;               \
    ss << msg;                          \
    Logger::QueueLog(ss.str(), Logger::Context{__FILE__, __LINE__, __func__, #cat,    \
            Logger::Kind::ERROR});         \
}

#define _logWarning(cat, msg)           \
{                                       \
    using namespace mozi::core;         \
    if (Logger::GetVerbosity() >= Logger::Kind::WARNING) \
    {                                   \
        std::stringstream ss;           \
        ss << msg;                      \
        Logger::QueueLog(ss.srt(), Logger::Context(__FILE__, __LINE__, __func__,      \
            #cat, Logger::Kind::WARNING)); \
    }                                   \
}                                       \

#define _logInfo(cat, msg)           \
{                                       \
    using namespace mozi::core;         \
    if (Logger::GetVerbosity() >= Logger::Kind::INFO) \
    {                                   \
        std::stringstream ss;           \
        ss << msg;                      \
        Logger::QueueLog(ss.srt(), Logger::Context(__FILE__, __LINE__, __func__,      \
            #cat, Logger::Kind::INFO)); \
    }                                   \
}  
} // namespace log
} // namespace core
} // namespace mozi

#endif
