#ifndef _LATTICE_LOG_H
#define _LATTICE_LOG_H

#include <string>
#include <format>
#include <iostream>
#include <iomanip>
#include <source_location>
#include <thread>

namespace lattice{

class Log {
    static constexpr size_t NUM_OF_SERVERITIES = 7;
public:
    static constexpr unsigned int TRACE = 1;
    static constexpr unsigned int DEBUG = 2;
    static constexpr unsigned int INFO = 3;
    static constexpr unsigned int WARN = 4;
    static constexpr unsigned int ERROR = 5;
    static constexpr unsigned int FATAL = 6;

    static const std::string severities[NUM_OF_SERVERITIES];

    /**
     * This data type is used to keeping source location which will eventally be written into log.
     */
    struct LogSource {
        std::string_view        msg;
        std::source_location    location;

        LogSource(char const * msg, 
            std::source_location location = std::source_location::current()) : msg{msg}, location{location}
        {}

        LogSource(const std::string& msg, 
            std::source_location location = std::source_location::current()) : msg{msg}, location{location}
        {}
    };

    static Log trace;
    static Log info;
    static Log debug;
    static Log warn;
    static Log error;
    static Log fatal;    

    Log() : Log{INFO} {}

    Log(unsigned int severity) : severity_{severity} {}

    static void setLogLevel(unsigned int level) {
        if (level < NUM_OF_SERVERITIES)
            logLevel_ = level; 
    }

    static unsigned int getLogLevel() {
        return logLevel_;
    } 

    template<typename... Args>
    void operator()(const LogSource& f, Args&&... args) {
        log(this->severity_, f.location.function_name(), f.location.line(), f.msg, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void log(const std::string& f, Args&&... args) {
        log(0, "", "", 0, f, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void log(unsigned int severity, 
                const std::string_view fnName, 
                int lineNo, 
                const std::string_view f, 
                Args&&... args) {
        if (severity < logLevel_) return;
        try {
            // get current time
            char buf[1000];
            time_t now = time(nullptr);
            struct tm tm;
            gmtime_r(&now, &tm);
            static const char* dateFormat = "%Y-%m-%d %H:%M:%S";
            int milli = now % 1000;
            strftime(buf, sizeof buf, dateFormat, &tm);

            std::ostringstream osstream;

            osstream << buf << "." << std::setfill('0') << std::setw(3) << milli;
            if (severity > 0 && severity < NUM_OF_SERVERITIES) {
                osstream << " [" << severities[severity] << "] ";
            } else osstream << " ";

            osstream << std::this_thread::get_id() << " ";

            if (fnName.size() > 0) {
                osstream << "[" << fnName << ":" << lineNo << "] ";
            }

            constexpr auto argsSize = sizeof...(Args);
            if (argsSize > 0) {
                try {
                    osstream << std::vformat(f, std::make_format_args(std::forward<Args>(args)...));
                } catch (...) {
                    osstream << f << " [";
                    ((osstream << args << ' '), ...);
                    osstream << "]";
                }
            } else {
                osstream << f;
            }

            osstream << "\n";
            
            std::cout << osstream.str();
        } catch (const std::exception& ex) {
            std::cout << ex.what() << std::endl;
        } catch (...) {
            // ignore
        }

    }

private:
    static std::string filePath;
    static unsigned int logLevel_;
    unsigned int severity_;
};

} // namespace lattice

#endif // _LATTICE_LOG_H