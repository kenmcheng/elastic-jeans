#include <elasticJeans/tcp/tcp_worker.hpp>
#include <elasticJeans/tcp/tcp_linux.hpp>
#include <elasticJeans/log/log.hpp>

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
        Log::trace("New connection built, _fd: {}", conn_socket_fd);
        Log::trace("accepted ip: {}", inet_ntoa(scaddr.sin_addr));
        Log::trace("accepted port: {}", ntohs(scaddr.sin_port));
        Connection conn{conn_socket_fd,
                         std::string{inet_ntoa(scaddr.sin_addr)},
                        (int) ntohs(scaddr.sin_port), 
                        tcpListener_->ipv4Address_, 
                        tcpListener_->port_};

        for (auto& cb : fnChain) {
            int code = (*cb)(conn);

            switch (code) {
                case PROCEED:
                    break;
                case INTERRUPT_WO_CLOSE:
                    conn.setAutoClose(false);
                    goto endloop;
                    break;
                case INTERRUPT_W_CLOSE:
                default:
                    close(conn_socket_fd);
                    goto endloop;
                    break;
            }
        }

    endloop:
        {}

    } catch (...) {
        Log::error("Failed to handle reuqest");
        close(conn_socket_fd);
    }
}

} // namespace tcp
} // namespace elasticJeans