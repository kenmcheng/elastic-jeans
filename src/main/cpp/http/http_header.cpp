#include "http_header.hpp"
#include <sstream>

namespace elasticJeans {
namespace http {

std::string& Headers::operator[](const std::string& headerName) {
    return this->get(headerName);
}

std::string& Headers::get(const std::string& headerName) {
    return headers_[headerName];
}

void Headers::set(std::string headerName, std::string value) {
    headers_[headerName] = move(value);
}

std::string Headers::str() {
    std::ostringstream osstream;
    for (auto& [k, v] : headers_) {
        osstream << k << ": "  << v << "\n";
    }
    return osstream.str();
}

} // http
} // namespace elasticJeans