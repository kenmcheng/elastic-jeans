#ifndef _ELASTICJEANS_HTTP_REQUEST_H
#define _ELASTICJEANS_HTTP_REQUEST_H

#include <elasticJeans/http/http_header.hpp>

#include <algorithm> 
#include <string>
#include <memory>

namespace elasticJeans {
namespace http {

class Method {
public:
    static constexpr int METHODS_LEN = 10;
    static const std::string METHODS[METHODS_LEN];

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

    MethodEnum value() const { return methodEnum_; }

    std::string str() { return METHODS[methodEnum_]; }

private:
    MethodEnum methodEnum_;

};

class HttpRequest {
    static const std::string REQUSET_DELIMITER_CRLF;
    static const std::string REQUSET_DELIMITER_LF;
    static const std::string HEADER_DELIMITER;
public:
    HttpRequest() = default;

    HttpRequest(const std::string& tcpData) { parse(tcpData); }

    void parse(const std::string& tcpData);

    void setMethod(const Method& method) { this->method_ =  method; }

    Method& getMethod() { return method_; }

    std::string& getPath() { return path_; }

    HeaderList& getHeaders() { return headers_; }

    void setPayload(const std::string& payload) { this->payload_ = payload; }

    std::string& getPayload() { return payload_; }

    std::string toString();
    
    void clear();

private:
    std::string path_;
    Method method_;
    HeaderList headers_;
    std::string payload_;

};

extern thread_local std::unique_ptr<HttpRequest> reqPtr;

} // namespace http
} // namespace elasticJeans

#endif // _ELASTICJEANS_HTTP_REQUEST_H