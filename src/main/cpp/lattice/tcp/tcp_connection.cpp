#include <lattice/tcp/tcp_connection.hpp>
#include <lattice/log/log.hpp>

#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>
#include <exception>

namespace lattice {
namespace tcp  {

Connection::Connection(int socket_fd, std::string initIP, int initPort, std::string recIP, int recPort) :
    conn_socket_fd_{socket_fd},
    initiatorIP_{initIP}, initiatorPort_{initPort},
    receiverIP_{recIP}, receiverPort_{recPort} {

}

Connection::~Connection() {
    if (autoClose_ && !closed_) fin();
}

std::string Connection::receiveData(ssize_t bufferSize) {
    std::ostringstream osstream;
    ssize_t bytesReceived = 0;
    char buffer[bufferSize];
    
    ssize_t fetched;
    int retries = 0;
    while ((fetched = recv(conn_socket_fd_, buffer, bufferSize, MSG_DONTWAIT)) > 0 || (bytesReceived == 0 && retries++ < 3)) {
        if (fetched <= 0) {
            Log::debug("Failed to read, retries: {}", retries);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        // Log::info("Read into buffer: " + std::to_string(fetched));
        osstream << std::string(buffer, fetched);
        bytesReceived += fetched;
    }
    
    if (bytesReceived == 0) {
        Log::error("Failed to read bytes from client socket connection, last read: {}", fetched);
        throw "Failed to read bytes from client socket connection";
    } else
        Log::info("Read bytes: {}", bytesReceived);

    return osstream.str();
}

void Connection::sendData(const std::string& data) {

    long bytesSent;

    bytesSent = write(conn_socket_fd_, data.c_str(), data.size());

    if (bytesSent == data.size()) {
        Log::info("------ Server response has been sent ------");
    } else {
        Log::info("Error occured when sending response");
    }
}

void Connection::fin() {
    closed_ = true;
    close(conn_socket_fd_);
}

} // namespace tcp
} // namespace lattice