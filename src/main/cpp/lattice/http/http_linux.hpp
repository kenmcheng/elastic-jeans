#ifndef _LATTICE_HTTP_LINUX_H
#define _LATTICE_HTTP_LINUX_H

#include <lattice/tcp/tcp_linux.hpp>
#include <lattice/tcp/tcp_connection.hpp>
// #include <lattice/http/http_request.hpp>
// #include <lattice/http/http_response.hpp>

#include <lattice/restful/rest_api_registry.hpp>

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <csignal>
#include <utility>
#include <algorithm>

namespace lattice {
namespace http {

extern thread_local std::unique_ptr<HttpRequest> request;
extern thread_local std::unique_ptr<HttpResponse> response;

class HttpServer {
public:
    HttpServer(std::string ipAddress = "127.0.0.1");
    // HttpServer(std::string ipAddress, int port, bool withSecure = false):
    //     ipAddress_(ipAddress),
    //     port_(port) {

    // }

    ~HttpServer();

    void http(int port);

    void http(std::string ipAddress, int port);

    void https(int port);

    void https(std::string ipAddress, int port);
    
    int start();

    int stop();

    int receive(tcp::Connection& tcpConnection);

    int handleRequest(const std::string& received);

    // RestApiRegistry& getRestApiRegister() { return apis; }

private:
    std::string ipAddress_;
    std::shared_ptr<tcp::Workers> tcpWorkers_;
    std::unique_ptr<ThreadPool> listenerThreadPool_;
    std::vector<std::unique_ptr<tcp::TcpListener>> listeners_;

    // RestApiRegistry apis;
};

} // http
} // lattice

#endif // _LATTICE_HTTP_LINUX_H