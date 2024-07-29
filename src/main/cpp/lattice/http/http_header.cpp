#include "lattice/http/http_header.hpp"
#include <sstream>

namespace lattice {
namespace http {

Header& Header::operator=(std::string value) {
    this->values_.push_back(std::move(value));
    return *this;
}

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
            osstream << k << ": "  << v << CRLF;
    }
    return osstream.str();
}

void HeaderList::clear() {
    headers_.clear();
}

} // http
} // namespace lattice