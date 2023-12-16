#ifndef _ELASTICJEANS_HTTP_LINUX_H
#define _ELASTICJEANS_HTTP_LINUX_H

#include <elasticJeans/tcp/tcp_linux.hpp>
#include <elasticJeans/tcp/tcp_connection.hpp>
// #include <elasticJeans/http/http_request.hpp>
// #include <elasticJeans/http/http_response.hpp>

#include <elasticJeans/restful/rest_api_registry.hpp>

// #include <memory>
#include <string>
#include <iostream>
#include <csignal>

namespace elasticJeans {
namespace http {

extern thread_local std::unique_ptr<HttpRequest> reqPtr;
extern thread_local std::unique_ptr<HttpResponse> respPtr;

class HttpServer {
public:
    HttpServer(std::string ipAddress, int port, bool withSecure = false):
        ipAddress_(ipAddress),
        port_(port),
        tcp_(ipAddress, port),
        withSecure_(withSecure) {
        
    }

    ~HttpServer() = default;
    
    int start();

    int receive(tcp::Connection& tcpConnection);

    int tlsHandshake(tcp::Connection& tcpConnection);

    // RestApiRegistry& getRestApiRegister() { return apis; }

private:
    std::string ipAddress_;
    int port_;
    tcp::TcpListener tcp_;
    bool withSecure_;

    // RestApiRegistry apis;
};

} // http
} // elasticJeans

#endif // _ELASTICJEANS_HTTP_LINUX_H