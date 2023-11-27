#include "app.hpp"

namespace elasticJeans {

// std::unique_ptr<http::HttpServer> serverPtr;
std::unique_ptr<App> appPtr;

void sigFunc(int signal) {
    appPtr.reset();
}

App::Initiator::Initiator() {
    appPtr = std::make_unique<App>();
}

void App::Initiator::start() {
    appPtr->start();
}

App::Initiator& App::Initiator::http(const std::string& ip, int port) {
    appPtr->http(ip, port);
    return *this;
}

App::Initiator& App::Initiator::https(const std::string& ip, int port) {
    appPtr->https(ip, port);
    return *this;
}

void App::start() {
    for (auto& server: serverPtrs_) {
        server->start();
    }
}

App& App::http(const std::string& ip, int port) {
    this->serverPtrs_.push_back(std::make_unique<http::HttpServer>(ip, port));
    return *this;
}

App& App::https(const std::string& ip, int port) {
    this->serverPtrs_.push_back(std::make_unique<http::HttpServer>(ip, port, true));
    return *this;
}

} // namespace elasticJeans