#ifndef _ELASTICJEANS_HTTP_LINUX_H
#define _ELASTICJEANS_HTTP_LINUX_H

#include <tcp/tcp_linux.hpp>
#include <tcp/tcp_connection.hpp>
#include "http_request.hpp"
#include "http_response.hpp"

#include <restful/rest_api_register.hpp>

#include <memory>
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

    RestApiRegister& getRestApiRegister() { return apis; }

private:
    std::string ipAddress_;
    int port_;
    tcp::TcpListener tcp_;
    bool withSecure_;

    RestApiRegister apis;
};

} // http
} // elasticJeans

#endif // _ELASTICJEANS_HTTP_LINUX_H