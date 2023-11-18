#include "http_linux.hpp"
#include <log/log.hpp>
#include "http_header.hpp"

#include <string>
#include <time.h>

namespace elasticJeans {
namespace http {

thread_local HttpRequest req;
thread_local HttpResponse resp;

int HttpServer::start() {
    Log::info(ipAddress_ + ":" + std::to_string(port_) );
    tcp_.registerCbFunc([this](tcp::Connection& tcpConnection) -> int {
        return receive(tcpConnection);
    });

    tcp_.start();
    return 0;
}

int HttpServer::receive(tcp::Connection& tcpConnection) {
    std::string received = tcpConnection.receiveData();

    try {
        req.parse(received);
    } catch (std::exception e) {
        return 0;
    }

    auto apiOpt = apis.lookFor(req.getMethod(), req.getPath());

    if (apiOpt.has_value()) {
        apiOpt->get()->invoke();
    } else {
        resp.setStatus(404);
    }

    tcpConnection.sendData(resp.prepare());
    resp.clear();
    return 0;
}

} // namespace http
} // namespace elasticJeans
