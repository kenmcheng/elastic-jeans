#include "http_request.hpp"
#include <util/string_util.hpp>
#include <log/log.hpp>

#include <string>
#include <vector>

namespace elasticJeans {
namespace http {

const std::string HttpRequest::REQUSET_DELIMITER_CRLF = "\r\n\r\n";
const std::string HttpRequest::REQUSET_DELIMITER_LF = "\n\n";
const std::string HttpRequest::HEADER_DELIMITER = ": ";

const std::string Method::METHODS[METHODS_LEN] = {
    "GET",
    "HEAD",
    "POST",
    "PUT",
    "DELETE",
    "CONNECT",
    "OPTIONS",
    "TRACE",
    "PATCH",
    "UNDEFINED"
};

void HttpRequest::parse(const std::string& tcpData) {
    int splitPt = tcpData.find(REQUSET_DELIMITER_CRLF);
    if (splitPt == std::string::npos) {
        splitPt = tcpData.find(REQUSET_DELIMITER_LF);
    }

    std::vector<std::string> headerTokens = tokenize(tcpData, CRLF, false, 0, splitPt);
    if (headerTokens.size() == 0) return;

    std::vector<std::string> requsetLine = tokenize(headerTokens[0], " ");
    if (requsetLine.size() != 3) return;
    method_.str(requsetLine[0]);
    path_ = requsetLine[1];

    int sz = headerTokens.size();
    int idx = 1;

    // fetch headers
    for (; idx < sz; idx++) {
        if (headerTokens[idx].size() <= 1) break;
        std::vector<std::string> header = tokenize(headerTokens[idx], HEADER_DELIMITER);
        // Log::trace(headerTokens[idx]);
        if (header.size() < 2 ) continue;
        headers_[std::move(header.at(0))] = std::move(header.at(1));
    }

    // fetch payload
    payload_ = tcpData.substr(splitPt + REQUSET_DELIMITER_CRLF.size());
}

std::string HttpRequest::toString() {
    return method_.str() + " " + path_;
}

} // namespace http
} // namespace elasticJeans

