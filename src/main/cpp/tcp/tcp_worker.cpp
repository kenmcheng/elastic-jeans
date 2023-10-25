#include "tcp_worker.hpp"
#include "../log/log.hpp"

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>
#include <exception>


namespace elasticJeans {
namespace tcp {

void Workers::handle(int conn_socket_fd) {
    auto fn = [this] (int conn_socket_fd) -> void {
        doHandle(conn_socket_fd);
    };
    threadPoolExecutor_->submit(std::move(fn), conn_socket_fd);
}

void Workers::doHandle(int conn_socket_fd) {
    Log::info("New request received, _fd: " + std::to_string(conn_socket_fd));
    bool fin = false;
    try {
        while (!fin) {
            
            ssize_t bytesReceived = 0;
            char buffer[bufferSize_];

            std::ostringstream osstream;
            ssize_t fetched;
            int retries = 0;
            while ((fetched = recv(conn_socket_fd, buffer, bufferSize_, MSG_DONTWAIT)) > 0 || (bytesReceived == 0 && retries++ < 3)) {
                if (fetched <= 0) {
                    Log::debug("Failed to read, retries: " + std::to_string(retries));
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    continue;
                }
                Log::info("Read into buffer: " + std::to_string(fetched));
                osstream << std::string(buffer, fetched);
                bytesReceived += fetched;
            }
            
            if (bytesReceived == 0) {
                Log::error("Failed to read bytes from client socket connection, last read: " + std::to_string(fetched));
                close(conn_socket_fd);
                return;
            }
            Log::log(osstream.str());
            Log::info("Read bytes: " + std::to_string(bytesReceived));

            std::string req = osstream.str();
            std::string resp = "";

            for (auto& cb : fnChain) {
                int action = (*cb)(req, resp);
            }

            sendResponse(conn_socket_fd, resp);
            fin = true;
        }
    } catch (std::exception e) {
        Log::error("Failed to handle reuqest");
        fin = true;
    }

    close(conn_socket_fd);
}

void Workers::sendResponse(int conn_socket_fd, const std::string& response) {

    long bytesSent;

    bytesSent = write(conn_socket_fd, response.c_str(), response.size());

    if (bytesSent == response.size()) {
        Log::info("------ Server response has been sent ------");
    } else {
        Log::info("Error occured when sending response");
    }
}

} // namespace tcp
} // namespace elasticJeans