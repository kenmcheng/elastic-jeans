#include <lattice/tcp/tcp_linux.hpp>
#include <lattice/log/log.hpp>
#include <lattice/http/http_header.hpp>

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <chrono>
#include <thread>

namespace lattice {
namespace tcp {

// TcpListener::TcpListener(std::string ipAddress, int port, int workerPoolSize, std::unique_ptr<TcpHandler> handler):
//         ipv4Address_(ipAddress), 
//         port_(port),
//         socketAddress_len_(sizeof(socketAddress_)) {
//     handler_ = move(handler);
//     handler_->setTcpListener(this);
//     workers_ = std::make_shared<Workers>(workerPoolSize);

//     socketAddress_.sin_family = AF_INET;
//     socketAddress_.sin_port = htons(port_);
//     socketAddress_.sin_addr.s_addr = inet_addr(ipv4Address_.c_str());
    
//     this->init();
// }

TcpListener::TcpListener(
    std::string ipAddress, 
    int port,
    std::shared_ptr<Workers> workers,
    std::unique_ptr<TcpHandler> handler
    ): 
        ipv4Address_(ipAddress), 
        port_(port), 
        socketAddress_len_(sizeof(socketAddress_)), 
        handler_{move(handler)}, 
        workers_{workers} {
    
    handler_->setTcpListener(this);

    socketAddress_.sin_family = AF_INET;
    socketAddress_.sin_port = htons(port_);
    socketAddress_.sin_addr.s_addr = inet_addr(ipv4Address_.c_str());
    
    this->init();
}

TcpListener::~TcpListener() {
    stop();
}

int TcpListener::init() {
    if (init_== true) return 0;
    init_ = true;

    socket_ipv4_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ipv4_fd_ < 0) {
        Log::error("Failed to start TCP connector");
        exit(1);
        return 1;
    }
    int retries = 0;
    int retryLimit = 30;
    while (retries < retryLimit) {
        if (bind(socket_ipv4_fd_, (sockaddr *) &socketAddress_, socketAddress_len_) == 0)
            break;
        retries++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    if (retries == retryLimit) {
        Log::error("Unable to bind socket");
        exit(1);
    }
    return 0;
}

void TcpListener::start() {
    this->_listen_ipv4();
}

void TcpListener::stop() {
    if (stop_) return;
    stop_ = true;
    shutdown(socket_ipv4_fd_, SHUT_RDWR);
    close(socket_ipv4_fd_);
}

void TcpListener::_listen_ipv4() {
    int rtn = listen(socket_ipv4_fd_, socketQueueSize_);
    if (rtn < 0) {
        Log::info("Failed to listen on {}:{}", inet_ntoa(socketAddress_.sin_addr), ntohs(socketAddress_.sin_port));
        return;
    }
    
    Log::info("*** Listening on {}:{} ***", inet_ntoa(socketAddress_.sin_addr), ntohs(socketAddress_.sin_port));
    while (true) {
        int conn_socket_fd = this->_accept_ipv4();

        if (conn_socket_fd < 0) {
            Log::warn("socket connection error. _fd: {}", std::to_string(conn_socket_fd));
            close(conn_socket_fd);
            if (stop_) break;
            else continue;
        }

        workers_->handle([this, conn_socket_fd]() {
            handler_->doHandle(conn_socket_fd, socketAddress_);
        });
    }
}

int TcpListener::_accept_ipv4() noexcept {
    return accept(socket_ipv4_fd_, (sockaddr *) &socketAddress_, &socketAddress_len_);
}


} // namespace tcp
} // namespace lattice