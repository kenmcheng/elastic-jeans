#ifndef _ELASTICJEANS_HTTP_REQUEST_H
#define _ELASTICJEANS_HTTP_REQUEST_H

#include "http_header.hpp"

#include <algorithm> 
#include <string>

namespace elasticJeans {
namespace http {

class Method {
    static const int METHODS_LEN = 10;
    static const std::string METHODS[METHODS_LEN];

public:
    enum MethodEnum {
        GET,
        HEAD,
        POST,
        PUT,
        DELETE,
        CONNECT,
        OPTIONS,
        TRACE,
        PATCH,
        UNDEFINED
    };

    Method() = default;

    Method(MethodEnum m) : methodEnum_(m) {}

    Method(std::string methodStr) {
        this->str(methodStr);
    }

    constexpr bool operator==(Method m) const { return methodEnum_ == m.methodEnum_; }

    constexpr bool operator!=(Method m) const { return methodEnum_ != m.methodEnum_; }

    void str(std::string methodStr) {
        auto mp = std::find(METHODS, METHODS+METHODS_LEN, methodStr);
        if (mp  == end(METHODS)) {
            methodEnum_ = MethodEnum::UNDEFINED;
        } else
            methodEnum_ = MethodEnum(mp-begin(METHODS));
    }

    std::string str() { return METHODS[methodEnum_]; }

private:
    MethodEnum methodEnum_;

};

class HttpRequest {
    static const std::string REQUSET_DELIMITER;
    static const std::string HEADER_DELIMITER;
public:
    HttpRequest() = default;

    HttpRequest(const std::string& tcpData) { parse(tcpData); }

    void parse(const std::string& tcpData);

    HeaderList& getHeaders() { return headers_; }

    std::string toString();

private:
    std::string path_;
    Method method_;
    HeaderList headers_;
    std::string payload_;

};

} // namespace http
} // namespace elasticJeans

#endif // _ELASTICJEANS_HTTP_REQUEST_H