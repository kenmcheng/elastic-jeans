#include "tcp_linux.hpp"
#include "../log/log.hpp"
#include "../http/http_header.hpp"

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>

namespace elasticJeans {
namespace tcp {

TcpListener::TcpListener(std::string ipAddress, int port, int workerPoolSize):
        ipAddress_(ipAddress), 
        port_(port),
        socketAddress_len_(sizeof(socketAddress_)),
        workers_(workerPoolSize) {
    
    socketAddress_.sin_family = AF_INET;
    socketAddress_.sin_port = htons(port_);
    socketAddress_.sin_addr.s_addr = inet_addr(ipAddress_.c_str());
    this->init();
}

TcpListener::~TcpListener() {
    Log::info("Server stopping...");
    stop();
}

int TcpListener::init() {
    socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd_ < 0) {
        Log::error("Failed to start TCP connector");
        return 1;
    }
    bind(socket_fd_, (sockaddr *) &socketAddress_, socketAddress_len_);
    return 0;
}

void TcpListener::start() {
    this->_listen();
}

void TcpListener::stop() {
    stop_ = true;
    close(socket_fd_);
}

void TcpListener::_listen() {
    listen(socket_fd_, 30);
    std::ostringstream osstream;
    osstream << "*** Listening on ADDRESS: " << inet_ntoa(socketAddress_.sin_addr) << " PORT: " << ntohs(socketAddress_.sin_port) << " ***\n\n";
    Log::info(osstream.str());

    
    while (true) {
        int conn_socket_fd = this->_accept();

        if (conn_socket_fd < 0) {
            Log::warn("socket connection error. _fd: " + std::to_string(conn_socket_fd));
            close(conn_socket_fd);
            if (stop_) break;
            else continue;
        }

        workers_.handle(conn_socket_fd);
    }
}

int TcpListener::_accept() {
    return accept(socket_fd_, (sockaddr *) &socketAddress_, &socketAddress_len_);
}


} // namespace tcp
} // namespace elasticJeans