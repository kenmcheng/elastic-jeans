#include <lattice/http/http_linux.hpp>
#include <lattice/http/http_thread_local.hpp>
#include <lattice/log/log.hpp>
#include <lattice/http/http_header.hpp>

#include <lattice/tls/handshaker.hpp>

#include <time.h>

namespace lattice {
namespace http {

extern thread_local std::unique_ptr<HttpRequest> request;
extern thread_local std::unique_ptr<HttpResponse> response;

HttpServer::HttpServer(std::string ipAddress)
: ipAddress_{ipAddress} {
    tcpWorkers_ = std::make_shared<tcp::Workers>();
    listenerThreadPool_ = std::make_unique<ThreadPool>();
}

HttpServer::~HttpServer() {
    stop();
}

void HttpServer::http(int port) {
    http(ipAddress_, port);
}

void HttpServer::http(std::string ipAddress, int port) {
    listeners_.emplace_back(
        std::make_unique<tcp::TcpListener>(
            ipAddress, 
            port,
            tcpWorkers_)
    );
}

void HttpServer::https(int port) {
    https(ipAddress_, port);
}

void HttpServer::https(std::string ipAddress, int port) {
    listeners_.emplace_back(
        std::make_unique<tcp::TcpListener>(
            ipAddress, 
            port, 
            tcpWorkers_, 
            std::make_unique<tls::Handshaker>())
    );
}


int HttpServer::start() {
    tcpWorkers_->run();

    for (auto& listrPtr : listeners_) {
        listrPtr->registerCbFunc([this](tcp::Connection& tcpConnection) -> int {
            return receive(tcpConnection);
        });

        listenerThreadPool_->add(std::thread([&listrPtr] {
            listrPtr->start();
        }));
    }
    return 0;
}

int HttpServer::stop() {
    for (auto& listener : listeners_) {
        listener->stop();
    }
    listenerThreadPool_->terminate();
    return 0;
}

int HttpServer::receive(tcp::Connection& tcpConnection) {
    std::string received = tcpConnection.receiveData();
    request = std::make_unique<HttpRequest>();
    response = std::make_unique<HttpResponse>();

    handleRequest(received);

    tcpConnection.sendData(response->prepare());

    request.reset();
    response.reset();
    return 0;
}

int HttpServer::handleRequest(const std::string& received) {
    try {
        request->parse(received);

        auto apiPtrOpt = RestApiRegistry::getInstance().lookFor(request->getMethod(), request->getPath());
        if (apiPtrOpt.has_value()) {
            apiPtrOpt->get()->invoke();
        } else {
            response->setStatus(404);
        }
    } catch(std::exception& ex) {
        response->setStatus(500);
        response->setContent("Internal server error");
    } catch(...) {
        response->setStatus(500);
        response->setContent("Internal server error");
    }

    return 0;
}


} // namespace http
} // namespace lattice
