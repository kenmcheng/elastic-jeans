#include "http_linux.hpp"
#include "../log/log.hpp"

namespace elasticJeans {
namespace http {

int HttpServer::start() {
    Log::info(m_ipAddress + ":" + std::to_string(m_port) );
    tcp.start();
    return 0;
}

} // namespace http
} // namespace elasticJeans
