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
        m_ipAddress(ipAddress),
        m_port(port),
        tcp(ipAddress, port) {

    };

    ~HttpServer() = default;
    
    int start();

private:
    std::string m_ipAddress;
    int m_port;
    tcp::TcpListener tcp;
};

} // http
} // elasticJeans

#endif // _ELASTICJEANS_HTTP_LINUX_H