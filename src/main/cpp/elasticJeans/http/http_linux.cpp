#include "http_linux.hpp"
#include <log/log.hpp>
#include "http_header.hpp"

#include <tls/handshaker.hpp>

#include <time.h>

namespace elasticJeans {
namespace http {

thread_local std::unique_ptr<HttpRequest> reqPtr;
thread_local std::unique_ptr<HttpResponse> respPtr;

int HttpServer::start() {
    Log::info(ipAddress_ + ":" + std::to_string(port_) );

    if (withSecure_) {
        tcp_.registerCbFunc([this](tcp::Connection& tcpConnection) -> int {
            return tlsHandshake(tcpConnection);
        });
    }

    tcp_.registerCbFunc([this](tcp::Connection& tcpConnection) -> int {
        return receive(tcpConnection);
    });

    tcp_.start();
    return 0;
}

int HttpServer::receive(tcp::Connection& tcpConnection) {
    std::string received = tcpConnection.receiveData();
    reqPtr = std::make_unique<HttpRequest>();
    respPtr = std::make_unique<HttpResponse>();

    try {
        reqPtr->parse(received);
    } catch (std::exception e) {
        return 0;
    }

    auto apiOpt = apis.lookFor(reqPtr->getMethod(), reqPtr->getPath());

    if (apiOpt.has_value()) {
        apiOpt->get()->invoke();
    } else {
        respPtr->setStatus(404);
    }

    tcpConnection.sendData(respPtr->prepare());
    reqPtr.reset();
    respPtr.reset();
    return 0;
}

int HttpServer::tlsHandshake(tcp::Connection& tcpConnection) {
    std::string received = tcpConnection.receiveData();
    tls::Handshaker handshaker;
    handshaker.hello(received);
    return 0;
}

} // namespace http
} // namespace elasticJeans
