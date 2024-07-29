#include "app_impl.hpp"
#include <lattice/log/log.hpp>
namespace lattice {

AppImpl::AppImpl() {
    serverPtr_ = std::make_unique<http::HttpServer>();
}

AppImpl::~AppImpl() {
    stop();
}

void AppImpl::start() {
    stop_ = false;
    this->serverPtr_->start();

    std::unique_lock lock(mtx_);
    cv_.wait(lock, [this] { 
        return stop_; 
    });
}

void AppImpl::stop() {
    {
        std::unique_lock lock(mtx_);
        if (stop_) return;
        stop_ = true;
    }
    cv_.notify_all();
}

App& AppImpl::http(const std::string& ip, int port) {
    this->serverPtr_->http(ip, port);
    return *this;
}

App& AppImpl::https(const std::string& ip, int port) {
    this->serverPtr_->https(ip, port);
    return *this;
}

} // namespace lattice