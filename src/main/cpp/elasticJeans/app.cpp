#include <elasticJeans/app.hpp>
#include <elasticJeans/app_impl.hpp>

#include <csignal>
#include <memory>
#include <thread>

namespace elasticJeans {

std::unique_ptr<App> appPtr;
std::unique_ptr<std::thread> runner;

void sigFunc(int signal) {
    appPtr.reset();
}

App::Initiator app() {
    std::signal(SIGINT, sigFunc);
    return App::init();
}

App::Initiator app(const std::string& ip, int port, bool withTLS) {
    App::Initiator newApp = app();
    if (withTLS)
        newApp.https(ip, port);
    else
        newApp.http(ip, port);
    return newApp;
}

App::Initiator::Initiator() {
    appPtr = std::make_unique<AppImpl>();
}

void App::Initiator::start() {
    runner = std::make_unique<std::thread>([] {
        appPtr->start();
    });
    runner->join();
}

App::Initiator& App::Initiator::http(const std::string& ip, int port) {
    appPtr->http(ip, port);
    return *this;
}

App::Initiator& App::Initiator::https(const std::string& ip, int port) {
    appPtr->https(ip, port);
    return *this;
}

// void App::start() {
//     for (auto& server: serverPtrs_) {
//         server->start();
//     }
// }

// App& App::http(const std::string& ip, int port) {
//     this->serverPtrs_.push_back(std::make_unique<http::HttpServer>(ip, port));
//     return *this;
// }

// App& App::https(const std::string& ip, int port) {
//     this->serverPtrs_.push_back(std::make_unique<http::HttpServer>(ip, port, true));
//     return *this;
// }

} // namespace elasticJeans