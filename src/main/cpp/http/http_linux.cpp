#include "http_linux.hpp"
#include "../log/log.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "http_header.hpp"

#include <string>
#include <sstream>

namespace elasticJeans {
namespace http {

int HttpServer::start() {
    Log::info(ipAddress_ + ":" + std::to_string(port_) );
    tcp_.registerCbFunc([](std::string& req, std::string& resp) -> void {
        std::string content =  "200 ok 成功!!!";

        http::Headers headers;

        headers["Content-Type"] = "text/plain; charset=utf-8";
        headers["Content-Length"] = std::to_string(content.size());
        
        std::ostringstream osstream;
        osstream << "HTTP/1.1 200 OK\n" << headers.str() << "\n"
            << content;
        resp = osstream.str();
    });

    tcp_.start();
    return 0;
}

} // namespace http
} // namespace elasticJeans
