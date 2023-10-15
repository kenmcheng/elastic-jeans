#ifndef _ELASTICJEANS_LOG_H
#define _ELASTICJEANS_LOG_H

#include <string>

namespace elasticJeans{

class Log {
public:
    const static int TRACE = 1;
    const static int DEBUG = 2;
    const static int INFO = 3;
    const static int WARN = 4;
    const static int ERROR = 5;
    const static int FATAL = 6;

    static void trace(const std::string& message);
    static void info(const std::string& message);
    static void debug(const std::string& message);
    static void warn(const std::string& message);
    static void error(const std::string& message);
    static void fatal(const std::string& message);
    static void log(const std::string& message);
private:
    static std::string filePath;
    int logLevel = INFO;

};

} // namespace elasticJeans

#endif // _ELASTICJEANS_LOG_H