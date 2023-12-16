#include "app_impl.hpp"

namespace elasticJeans {


void AppImpl::start() {
    for (auto& server: serverPtrs_) {
        server->start();
    }
}

App& AppImpl::http(const std::string& ip, int port) {
    this->serverPtrs_.push_back(std::make_unique<http::HttpServer>(ip, port));
    return *this;
}

App& AppImpl::https(const std::string& ip, int port) {
    this->serverPtrs_.push_back(std::make_unique<http::HttpServer>(ip, port, true));
    return *this;
}

} // namespace elasticJeans