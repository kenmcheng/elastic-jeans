#ifndef _ELASTICJEANS_HTTP_LINUX_H
#define _ELASTICJEANS_HTTP_LINUX_H

#include "../tcp/tcp_linux.hpp"

#include <string>
#include <iostream>

namespace elasticJeans {
namespace http {

class HttpServer {
public:
    HttpServer(std::string ipAddress, int port):
        ipAddress_(ipAddress),
        port_(port),
        tcp_(ipAddress, port) {
            
    }

    ~HttpServer() = default;
    
    int start();

private:
    std::string ipAddress_;
    int port_;
    tcp::TcpListener tcp_;
};

} // http
} // elasticJeans

#endif // _ELASTICJEANS_HTTP_LINUX_H