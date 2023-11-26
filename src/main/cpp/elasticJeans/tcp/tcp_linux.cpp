#include "tcp_linux.hpp"
#include <log/log.hpp>
#include <http/http_header.hpp>

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <chrono>
#include <thread>

namespace elasticJeans {
namespace tcp {

TcpListener::TcpListener(std::string ipAddress, int port, int workerPoolSize):
        ipv4Address_(ipAddress), 
        port_(port),
        socketAddress_len_(sizeof(socketAddress_)) {
    
    workers_ = std::make_unique<Workers>(this, workerPoolSize);

    socketAddress_.sin_family = AF_INET;
    socketAddress_.sin_port = htons(port_);
    socketAddress_.sin_addr.s_addr = inet_addr(ipv4Address_.c_str());
    
    this->init();
}

TcpListener::~TcpListener() {
    Log::info("Server stopping...");
    stop();
}

int TcpListener::init() {
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
    stop_ = true;
    close(socket_ipv4_fd_);
}

void TcpListener::_listen_ipv4() {
    listen(socket_ipv4_fd_, 30);
    std::ostringstream osstream;
    osstream << "*** Listening on ADDRESS: " << inet_ntoa(socketAddress_.sin_addr) << " PORT: " << ntohs(socketAddress_.sin_port) << " ***\n\n";
    Log::info(osstream.str());

    
    while (true) {
        int conn_socket_fd = this->_accept_ipv4();

        if (conn_socket_fd < 0) {
            Log::warn("socket connection error. _fd: " + std::to_string(conn_socket_fd));
            close(conn_socket_fd);
            if (stop_) break;
            else continue;
        }

        workers_->handle(conn_socket_fd, socketAddress_);
    }
}

int TcpListener::_accept_ipv4() {
    return accept(socket_ipv4_fd_, (sockaddr *) &socketAddress_, &socketAddress_len_);
}


} // namespace tcp
} // namespace elasticJeans