#ifndef _ELASTICJEANS_HTTP_RESPONSE_H
#define _ELASTICJEANS_HTTP_RESPONSE_H

#include "http_header.hpp"

#include <string>

namespace elasticJeans {
namespace http {

class HttpResponse {
public:

    Headers& getHeaders() { return headers_; }

    void setContent(std::string content);
    std::string toString();

private:
    int status = 200;
    Headers headers_;
    std::string content_;
};

} // namespace http
} // namespace elasticJeans

#endif