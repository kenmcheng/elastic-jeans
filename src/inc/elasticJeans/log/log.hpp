#ifndef _ELASTICJEANS_LOG_H
#define _ELASTICJEANS_LOG_H

#include <string>
#include <format>
#include <iostream>
#include <iomanip>

namespace elasticJeans{

class Log {
    static constexpr size_t NUM_OF_SERVERITIES = 7;
public:
    static constexpr int TRACE = 1;
    static constexpr int DEBUG = 2;
    static constexpr int INFO = 3;
    static constexpr int WARN = 4;
    static constexpr int ERROR = 5;
    static constexpr int FATAL = 6;

    static const std::string severities[NUM_OF_SERVERITIES];

    static Log trace;
    static Log info;
    static Log debug;
    static Log warn;
    static Log error;
    static Log fatal;

    Log() : Log(INFO) {}

    Log(int severity) : severity_(severity) {}

    static void setLogLevel(unsigned int level) {
        if (level < NUM_OF_SERVERITIES)
            logLevel_ = level; 
    }

    static unsigned int getLogLevel() {
        return logLevel_;
    } 

    template<typename... Args>
    void operator()(const std::string& f, Args&&... args) {
        log(this->severity_, f, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void log(const std::string& f, Args&&... args) {
        log(0, f, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void log(int severity, const std::string& f, Args&&... args) {
        if (severity < logLevel_) return;
        try {
            char buf[1000];
            time_t now = time(0);
            struct tm tm = *gmtime(&now);
            static const char* dateFormat = "%Y-%m-%d %H:%M:%S";
            int milli = now % 1000;
            strftime(buf, sizeof buf, dateFormat, &tm);

            std::ostringstream osstream;

            osstream << buf << "." << std::setfill('0') << std::setw(3) << milli;
            if (severity > 0 && severity < NUM_OF_SERVERITIES) {
                osstream << " [" << severities[severity] << "] ";
            } else osstream << " ";

            constexpr auto argsSize = sizeof...(Args);
            if (argsSize > 0) {
                osstream << std::vformat(f, std::make_format_args(std::forward<Args>(args)...));
            } else {
                osstream << f;
            }
            
            std::cout << osstream.str() << std::endl;
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

} // namespace elasticJeans

#endif // _ELASTICJEANS_LOG_H