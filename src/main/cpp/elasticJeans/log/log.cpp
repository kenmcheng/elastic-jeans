#include <elasticJeans/log/log.hpp>
#include <iostream>

namespace elasticJeans {

const std::string Log::severities[NUM_OF_SERVERITIES] {
    "",
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

int Log::logLevel_ = 0;

void Log::trace(const std::string& message) {
    // std::cout << "[TRACE] " << message << std::endl;
    log(TRACE, message);
}

void Log::debug(const std::string& message) {
    // std::cout << "[DEBUG] " << message << std::endl;
    log(DEBUG, message);
}

void Log::info(const std::string& message) {
    // std::cout << "[INFO] " << message << std::endl;
    log(INFO, message);
}

void Log::warn(const std::string& message) {
    // std::cout << "[WARN] " << message << std::endl;
    log(WARN, message);
}

void Log::error(const std::string& message) {
    // std::cout << "[ERROR] " << message << std::endl;
    log(ERROR, message);
}

void Log::fatal(const std::string& message) {
    // std::cout << "[FATAL] " << message << std::endl;
    log(FATAL, message);
}

// void Log::log(const std::string& message) {
//     std::cout << message << std::endl;
// }
} // namespace ElasticJean