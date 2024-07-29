#include <lattice/app.hpp>
#include <lattice/app_impl.hpp>

#include <lattice/log/log.hpp>

#include <csignal>
#include <memory>
#include <thread>

namespace lattice {

std::unique_ptr<App> appPtr;
std::unique_ptr<std::thread> runnerPtr;

void sigIntFunc(int signal) {
    Log::error("Starting to shut down application...");
    appPtr.reset();
}

void sigTermFunc(int signal) {
    Log::error("Starting to shut down application...");
    appPtr.reset();
}

void sigSegVFunc(int signal) {
    Log::error("Segmentation fault is signaled");
}

void handleSignal() {
    std::signal(SIGINT, sigIntFunc);
    std::signal(SIGTERM, sigTermFunc);
    std::signal(SIGSEGV, sigSegVFunc);
}

App::Initiator app() {
    handleSignal();
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
    runnerPtr = std::make_unique<std::thread>([] {
        appPtr->start();
    });
    runnerPtr->join();
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

} // namespace lattice