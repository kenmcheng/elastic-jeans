#ifndef _ELASTICJEANS_HTTP_REQUEST_H
#define _ELASTICJEANS_HTTP_REQUEST_H

#include "http_header.hpp"

#include <string>

namespace elasticJeans {
namespace http {

class HttpRequest {
public:
    HttpRequest() = default;

    HttpRequest(std::string req);

    void parse(std::string requset);

private:
    std::string url_;
    Headers headers_;

};

} // namespace http
} // namespace elasticJeans

#endif // _ELASTICJEANS_HTTP_REQUEST_H