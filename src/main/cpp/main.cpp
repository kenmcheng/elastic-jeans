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

int main(int argc, char *argv[]) {
    std::signal(SIGINT, sigFunc);
    std::string ip = argc >= 2 ? std::string{argv[1]} : "127.0.0.1";
    int port = argc >= 3 ? std::atoi(argv[2]) : 8080;
    server = std::make_unique<http::HttpServer>(ip, port);

    server->start();
    return 0;
}