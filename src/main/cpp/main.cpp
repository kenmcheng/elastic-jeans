#include "http/http_linux.hpp"
#include <stdio.h>
#include <iostream>
#include <csignal>
#include <memory>

using namespace elasticJeans;

std::unique_ptr<http::HttpServer> server;

void sigFunc(int signal) {
    server.reset();
}

int main() {
    std::signal(SIGINT, sigFunc);
    server = std::make_unique<http::HttpServer>("127.0.0.1", 8080);

    server->start();
    return 0;
}