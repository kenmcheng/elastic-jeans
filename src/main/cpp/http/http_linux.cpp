#include "http_linux.hpp"
#include <log/log.hpp>
#include "http_request.hpp"
#include "http_response.hpp"
#include "http_header.hpp"

#include <string>
#include <time.h>

namespace elasticJeans {
namespace http {

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

    HttpRequest req;
    HttpResponse resp;
    try {
        req.parse(received);
    } catch (std::exception e) {
        return 0;
    }

    std::string content = "<html><head><title>Denim EJ</title></head><body>200 ok successful!!!</body></html>";

    http::HeaderList& headers = resp.getHeaders();
    // headers["Content-Encoding"] = "gzip";
    headers["Server"] = "webserver";
    headers["Content-Type"] = "text/html;charset=UTF-8";
    headers["Set-Cookie"] = "EJ_SessionId=saiodhwaehlksaggaas; path=/";
    headers["Connection"] = "close";
    headers["Vary"] = "Accept-Encoding";
    headers["X-Frame-Options"] = "SAMEORIGIN";
 
    resp.setContent(std::move(content));
    tcpConnection.sendData(resp.prepare());
    return 0;
}

} // namespace http
} // namespace elasticJeans
