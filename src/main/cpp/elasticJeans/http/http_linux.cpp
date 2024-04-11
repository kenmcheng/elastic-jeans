#include <elasticJeans/http/http_linux.hpp>
#include <elasticJeans/http/http_thread_local.hpp>
#include <elasticJeans/log/log.hpp>
#include <elasticJeans/http/http_header.hpp>

#include <elasticJeans/tls/handshaker.hpp>

#include <time.h>

namespace elasticJeans {
namespace http {

extern thread_local std::unique_ptr<HttpRequest> reqPtr;
extern thread_local std::unique_ptr<HttpResponse> respPtr;

int HttpServer::start() {
    // Log::info("{}:{}", ipAddress_ , std::to_string(port_));

    // if (withSecure_) {
    //     tcp_.registerCbFunc([this](tcp::Connection& tcpConnection) -> int {
    //         return tlsHandshake(tcpConnection);
    //     });
    // }

    // tcp_.registerCbFunc([this](tcp::Connection& tcpConnection) -> int {
    //     return receive(tcpConnection);
    // });
    
    // tcp_.start();

    for (auto& listrPtr : listeners_) {
        listrPtr->registerCbFunc([this](tcp::Connection& tcpConnection) -> int {
            return receive(tcpConnection);
        });

        listrPtr->start();
    }
    return 0;
}

int HttpServer::receive(tcp::Connection& tcpConnection) {
    std::string received = tcpConnection.receiveData();
    reqPtr = std::make_unique<HttpRequest>();
    respPtr = std::make_unique<HttpResponse>();

    handleRequest(received);

    tcpConnection.sendData(respPtr->prepare());

    reqPtr.reset();
    respPtr.reset();
    return 0;
}

int HttpServer::handleRequest(const std::string& received) {
    try {
        reqPtr->parse(received);

        auto apiPtrOpt = RestApiRegistry::getInstance().lookFor(reqPtr->getMethod(), reqPtr->getPath());
        if (apiPtrOpt.has_value()) {
            apiPtrOpt->get()->invoke();
        } else {
            respPtr->setStatus(404);
        }
    } catch(std::exception& ex) {
        respPtr->setStatus(500);
        respPtr->setContent("Internal server error");
    } catch(...) {
        respPtr->setStatus(500);
        respPtr->setContent("Internal server error");
    }

    return 0;
}


} // namespace http
} // namespace elasticJeans
