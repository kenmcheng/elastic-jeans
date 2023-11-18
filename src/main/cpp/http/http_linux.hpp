#ifndef _ELASTICJEANS_HTTP_LINUX_H
#define _ELASTICJEANS_HTTP_LINUX_H

#include <tcp/tcp_linux.hpp>
#include <tcp/tcp_connection.hpp>
#include "http_request.hpp"
#include "http_response.hpp"

#include <restful/rest_api_register.hpp>

#include <string>
#include <iostream>
#include <csignal>

namespace elasticJeans {
namespace http {

extern thread_local HttpRequest req;
extern thread_local HttpResponse resp;

class HttpServer {
public:
    HttpServer(std::string ipAddress, int port):
        ipAddress_(ipAddress),
        port_(port),
        tcp_(ipAddress, port) {
        
    }

    ~HttpServer() = default;
    
    int start();

    int receive(tcp::Connection& tcpConnection);

    RestApiRegister& getRestApiRegister() { return apis; }

private:
    std::string ipAddress_;
    int port_;
    tcp::TcpListener tcp_;

    RestApiRegister apis;
};

} // http
} // elasticJeans

#endif // _ELASTICJEANS_HTTP_LINUX_H