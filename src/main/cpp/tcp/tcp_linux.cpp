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
        socketAddress_len_(sizeof(socketAddress_)) {
    
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
    close(socket_fd_);
}

void TcpListener::_listen() {
    listen(socket_fd_, 30);
    std::ostringstream osstream;
    osstream << "*** Listening on ADDRESS: " << inet_ntoa(socketAddress_.sin_addr) << " PORT: " << ntohs(socketAddress_.sin_port) << " ***\n\n";
    Log::info(osstream.str());

    
    while (true) {
        int conn_socket_fd = this->_accept();
        
        Log::info("New request received, _fd: " + std::to_string(conn_socket_fd));

        ssize_t bytesReceived = 0;
        char buffer[bufferSize_];

        std::ostringstream osstream1;
        ssize_t fetched;
        int retries = 0;
        while ((fetched = recv(conn_socket_fd, buffer, bufferSize_, MSG_DONTWAIT)) > 0 || (bytesReceived == 0 && retries++ < 3)) {
            if (fetched <= 0) {
                Log::debug("Failed to read, retries: " + std::to_string(retries));
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }
            // fetched = read(_conn_socket_fd, buffer, bufferSize);
            Log::info("Read into buffer: " + std::to_string(fetched));
            osstream1 << std::string(buffer, fetched);
            bytesReceived += fetched;
        }
        
        if (bytesReceived == 0) {
            Log::error("Failed to read bytes from client socket connection, last read: " + std::to_string(fetched));
            close(conn_socket_fd);
            continue;
        }
        Log::log(osstream1.str());
        Log::info("Read bytes: " + std::to_string(bytesReceived));

        sendResponse(conn_socket_fd);
        close(conn_socket_fd);
    }
}

int TcpListener::_accept() {
    return accept(socket_fd_, (sockaddr *) &socketAddress_, &socketAddress_len_);
}

void TcpListener::sendResponse(int& _conn_socket_fd) {
    long bytesSent;
    std::string content = "200 ok 成功";
    http::Headers headers;

    headers.set("Content-Type", "text/plain; charset=utf-8");
    headers.set("Content-Length", std::to_string(content.size()));
    
    std::ostringstream osstream;
    osstream << "HTTP/1.1 200 OK\n" << headers.str() << "\n"
           << content;
    std::string resp = osstream.str();
    bytesSent = write(_conn_socket_fd, resp.c_str(), resp.size());

    if (bytesSent == resp.size()) {
        Log::info("------ Server Response sent to client ------");
    } else {
        Log::info("Error sending response to client");
    }
}

} // namespace tcp
} // namespace elasticJeans