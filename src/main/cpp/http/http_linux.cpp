#include "http_linux.hpp"
#include "../log/log.hpp"
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

    std::string content =  "<html><body>200 ok successful!!!</body></html>";

    char buf[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);

    http::Headers& headers = resp.getHeaders();

    headers["Server"] = "webserver";
    headers["Content-Type"] = "text/html; charset=utf-8";
    // headers["Content-Encoding"] = "gzip";
    // headers["Cache-Control"] = "private,no-store";
    headers["Set-Cookie"] = "EJ_SessionId=saiodhwaehlksaggaas";
    headers["Connection"] = "close";
    headers["Date"] = buf;
    headers["Vary"] = "Accept-Encoding";
    headers["X-Frame-Options"] = "SAMEORIGIN";
    // headers["Content-Length"] = std::to_string(content.size());
    resp.setContent(std::move(content));
    tcpConnection.sendData(resp.toString());
    return 0;
}

} // namespace http
} // namespace elasticJeans
