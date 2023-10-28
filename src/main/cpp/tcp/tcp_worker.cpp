#include "tcp_worker.hpp"
#include "tcp_linux.hpp"
#include "../log/log.hpp"

#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>
#include <exception>


namespace elasticJeans {
namespace tcp {

void Workers::handle(int conn_socket_fd, const sockaddr_in& scaddr) {
    auto fn = [this, conn_socket_fd, scaddr] () -> void {
        doHandle(conn_socket_fd, scaddr);
    };
    threadPoolExecutor_->submit(std::move(fn));
}

void Workers::doHandle(int conn_socket_fd, const sockaddr_in& scaddr) {
    try {
        Log::info("New connection built, _fd: " + std::to_string(conn_socket_fd));
        Log::info("accepted ip: " + std::string{inet_ntoa(scaddr.sin_addr)});
        Log::info("accepted port: " + std::to_string((int) ntohs(scaddr.sin_port)));
        Connection conn{conn_socket_fd,
                         std::string{inet_ntoa(scaddr.sin_addr)},
                        (int) ntohs(scaddr.sin_port), 
                        tcpListener_->ipv4Address_, 
                        tcpListener_->port_};

        for (auto& cb : fnChain) {
            int code = (*cb)(conn);
            if (code) {
                close(conn_socket_fd);
                break;
            }
        }

    } catch (...) {
        Log::error("Failed to handle reuqest");
        close(conn_socket_fd);
    }
}

} // namespace tcp
} // namespace elasticJeans