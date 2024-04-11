#include <elasticJeans/tls/secure_connection.hpp>
#include <elasticJeans/log/log.hpp>

#include <sstream>
#include <chrono>
#include <thread>

namespace elasticJeans {
namespace tls {

SecureConnection::SecureConnection(
    int socket_fd, 
    std::string initIP, 
    int initPort, 
    std::string recIP, 
    int recPort,
    std::unique_ptr<SSL, SSLDel> ssl
    ) : Connection(socket_fd, initIP, initPort, recIP, recPort) {
    
    ssl_ = move(ssl);
}

std::string SecureConnection::receiveData(ssize_t bufferSize) {
    std::ostringstream osstream;
    ssize_t bytesReceived = 0;
    char buffer[bufferSize];

    ssize_t fetched;
    int retries = 0;

    while ((fetched = SSL_read(ssl_.get(), buffer, sizeof(buffer))) > 0 || (bytesReceived == 0 && retries++ < 3)) {
        if (fetched <= 0) {
            Log::debug("Failed to read, retries: {}", retries);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        // Log::info("Read into buffer: " + std::to_string(fetched));
        osstream << std::string(buffer, fetched);
        bytesReceived += fetched;
        if (SSL_pending(ssl_.get()) <= 0) break;
    }
    if (bytesReceived == 0) {
        Log::error("Failed to read bytes from client secure connection, last read: {}", fetched);
        throw "Failed to read bytes from client secure connection";
    } else
        Log::info("Read bytes: {}", bytesReceived);

    return osstream.str();
}

void SecureConnection::sendData(const std::string& data) {

    long bytesSent;

    bytesSent = SSL_write(ssl_.get(), data.c_str(), data.size());

    if (bytesSent == data.size()) {
        Log::info("------ Server response has been sent ------");
    } else {
        Log::info("Error occured when sending response");
    }
}

} // tls
} // elasticJeans