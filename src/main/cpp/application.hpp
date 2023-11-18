#ifndef _ELASTICJEANS_APPLICATION_H
#define _ELASTICJEANS_APPLICATION_H

#include <http/http_linux.hpp>
#include <restful/rest.hpp>

#include <csignal>
#include <memory>

using elasticJeans::http::Method;

namespace elasticJeans {

extern std::unique_ptr<http::HttpServer> serverPtr;

inline void sigFunc(int signal) {
    serverPtr.reset();
}

inline void app(const std::string& ip, int port) {
    std::signal(SIGINT, sigFunc);
    serverPtr = std::make_unique<http::HttpServer>(ip, port);
}

inline void start() {
    serverPtr->start();
}


} // namespace elasticJeans

#endif