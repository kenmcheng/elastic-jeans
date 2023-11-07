#include "http_header.hpp"
#include <sstream>

namespace elasticJeans {
namespace http {

Header& HeaderList::operator[](const std::string& headerName) {
    return this->get(headerName);
}

Header& HeaderList::get(const std::string& headerName) {
    return headers_[headerName];
}

void HeaderList::set(std::string headerName, std::string value) {
    headers_[headerName] = std::move(value);
}

std::string HeaderList::str() const {
    std::ostringstream osstream;
    for (auto& [k, vals] : headers_) {
        for (auto& v : vals)
            osstream << k << ": "  << v << "\n";
    }
    return osstream.str();
}

} // http
} // namespace elasticJeans