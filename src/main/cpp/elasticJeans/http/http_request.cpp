#include <elasticJeans/http/http_request.hpp>
#include <elasticJeans/util/string_util.hpp>
#include <elasticJeans/log/log.hpp>

#include <string>
#include <vector>

namespace elasticJeans {
namespace http {

thread_local std::unique_ptr<HttpRequest> reqPtr;

const std::string HttpRequest::REQUSET_DELIMITER_CRLF = "\r\n\r\n";
const std::string HttpRequest::REQUSET_DELIMITER_LF = "\n\n";
const std::string HttpRequest::HEADER_DELIMITER = ":";

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

    std::vector<std::string> headerTokens = tokenize(tcpData, CRLF, false, true, 0, splitPt);
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
        std::string& header = headerTokens[idx];
        size_t delimIdx = header.find(HEADER_DELIMITER);
        // std::vector<std::string> header = tokenize(headerTokens[idx], HEADER_DELIMITER);
        if (delimIdx == std::string::npos)
            headers_[header] = "";
        else {
            std::string key = header.substr(0, delimIdx);
            size_t vStart = delimIdx+HEADER_DELIMITER.size();
            while (vStart < header.size() && header[vStart] == ' ') vStart++;
            std::string value = header.substr(vStart);
            headers_[std::move(key)] = std::move(value);
        }
            
    }
    // fetch payload
    payload_ = tcpData.substr(splitPt + REQUSET_DELIMITER_CRLF.size());
}

std::string HttpRequest::toString() {
    return method_.str() + " " + path_;
}

void HttpRequest::clear() {
    path_.clear();
    headers_.clear();
    payload_.clear();
}

} // namespace http
} // namespace elasticJeans

