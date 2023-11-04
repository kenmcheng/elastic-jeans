#include "http_response.hpp"

#include <sstream>

namespace elasticJeans {
namespace http {

void HttpResponse::setContent(std::string content) {
    headers_["Content-Length"] = std::to_string(content.size());
    this->content_ = std::move(content);
}

std::string HttpResponse::toString() {
    
    std::ostringstream osstream;
    osstream << "HTTP/1.1 200 OK\n" << headers_.str() << "\n"
        << content_;

    return osstream.str();
}

} // namespace http
} // namespace elasticJeans