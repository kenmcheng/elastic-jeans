#include <elasticJeans/http/http_response.hpp>

#include <sstream>
#include <time.h>

namespace elasticJeans {
namespace http {

thread_local std::unique_ptr<HttpResponse> respPtr;

void HttpResponse::setContent(std::string content) {
    headers_["Content-Length"] = std::to_string(content.size());
    this->content_ = std::move(content);
}

std::string HttpResponse::prepare() {
    if (!headers_.exist("Date"))
        setCurrentDate();

    return toString();
}

void HttpResponse::clear() {
    status_ = 200;
    content_.erase();
    headers_.clear();
}

std::string HttpResponse::toString() {
    
    std::ostringstream osstream;
    osstream << "HTTP/" << version_ << " " << status_;
    if (STATUS.count(status_))
        osstream <<  " " << STATUS[status_];

    osstream << CRLF << headers_.str() << CRLF
        << content_;

    return osstream.str();
}

void HttpResponse::setCurrentDate() {
    char buf[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    static const char* dateFormat = "%a, %d %b %Y %H:%M:%S %Z";
    strftime(buf, sizeof buf, dateFormat, &tm);

    headers_["Date"] = buf;
}

} // namespace http
} // namespace elasticJeans