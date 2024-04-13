#ifndef _ELASTICJEANS_HTTP_RESPONSE_H
#define _ELASTICJEANS_HTTP_RESPONSE_H

#include "http_header.hpp"

#include <string>
#include <unordered_map>
#include <memory>

namespace elasticJeans {
namespace http {

static std::unordered_map<int, std::string> STATUS {
    {200, "OK"},
    {400, "Bad Request"},
    {404, "Not Found"}
};

class HttpResponse {
public:

    HeaderList& getHeaders() { return headers_; }

    void setContent(std::string content);

    std::string getContent() { return content_; }

    void setCurrentDate();

    std::string prepare();

    void clear();

    std::string toString();

    void setStatus(int status) { status_= status; }

private:
    int status_ = 200;
    std::string version_ = "1.1";
    HeaderList headers_;
    std::string content_;
};

} // namespace http
} // namespace elasticJeans

#endif