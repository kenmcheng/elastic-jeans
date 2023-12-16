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
    const static int TRACE = 1;
    const static int DEBUG = 2;
    const static int INFO = 3;
    const static int WARN = 4;
    const static int ERROR = 5;
    const static int FATAL = 6;

    static const std::string severities[NUM_OF_SERVERITIES];

    // static Log fatal;

    static void trace(const std::string& message);
    static void info(const std::string& message);
    static void debug(const std::string& message);
    static void warn(const std::string& message);
    static void error(const std::string& message);
    static void fatal(const std::string& message);

    template<typename... Args>
    static void log(int severity, const std::string& f, Args&&... args) {
        if (severity < logLevel_) return;
        char buf[1000];
        time_t now = time(0);
        struct tm tm = *gmtime(&now);
        static const char* dateFormat = "%Y-%m-%d %H:%M:%S";
        int milli = now % 1000;
        strftime(buf, sizeof buf, dateFormat, &tm);

        std::cout << buf << "." << std::setfill('0') << std::setw(3) << milli;
        if (severity > 0 && severity < NUM_OF_SERVERITIES) {
            std::cout << " [" << severities[severity] << "] ";
        } else std::cout << " ";
        std::cout << std::vformat(f, std::make_format_args(std::forward<Args>(args)...));
        // log(args...);
        std::cout << std::endl;
    }

    // Log(int level) : logLevel_{level} {}

private:
    static std::string filePath;
    static int logLevel_;

};

} // namespace elasticJeans

#endif // _ELASTICJEANS_LOG_H