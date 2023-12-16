#include <elasticJeans/log/log.hpp>
#include <iostream>

namespace elasticJeans {

    void Log::trace(const std::string& message) {
        std::cout << "[TRACE] " << message << std::endl;
    }

    void Log::debug(const std::string& message) {
        std::cout << "[DEBUG] " << message << std::endl;
    }

    void Log::info(const std::string& message) {
        std::cout << "[INFO] " << message << std::endl;
    }

    void Log::warn(const std::string& message) {
        std::cout << "[WARN] " << message << std::endl;
    }

    void Log::error(const std::string& message) {
        std::cout << "[ERROR] " << message << std::endl;
    }

    void Log::fatal(const std::string& message) {
        std::cout << "[FATAL] " << message << std::endl;
    }

    void Log::log(const std::string& message) {
        std::cout << message << std::endl;
    }
} // namespace ElasticJean