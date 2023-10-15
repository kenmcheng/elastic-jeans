#include "http_header.hpp"
#include <sstream>

namespace elasticJeans {
namespace http {

std::string Headers::get(const std::string& headerName) {
    if (m_headers.count(headerName)) return m_headers[headerName];
    return "";
}

void Headers::set(std::string headerName, std::string value) {
    m_headers[headerName] = move(value);
}

std::string Headers::str() {
    std::ostringstream osstream;
    for (auto& [k, v] : m_headers) {
        osstream << k << ": "  << v << "\n";
    }
    return osstream.str();
}

} // http
} // namespace elasticJeans