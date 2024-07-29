#include <lattice/log/log.hpp>
#include <iostream>

namespace lattice {

const std::string Log::severities[NUM_OF_SERVERITIES] {
    "",
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

unsigned int Log::logLevel_ = 0;

Log Log::trace{TRACE};
Log Log::debug{DEBUG};
Log Log::info{INFO};
Log Log::warn{WARN};
Log Log::error{ERROR};
Log Log::fatal{FATAL};

} // namespace lattice