#ifndef _ELASTICJEANS_TCP_LINUX_H
#define _ELASTICJEANS_TCP_LINUX_H

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace elasticJeans {
namespace tcp {

class TcpListener {

public:
    TcpListener(std::string ipAddress = "127.0.0.1", int port = 8080, int workerPoolSize = 20);
    ~TcpListener();

    int init();
    void start();
    void stop();
    
    void sendResponse(int& _conn_socket_fd);

private:
    std::string ipAddress_;
    int port_;
    int socket_fd_;
    int bufferSize_ = 0X200;
    struct sockaddr_in socketAddress_;
    unsigned int socketAddress_len_;
    bool stop_ = false;

    void _listen();
    int _accept();
};

} // namespace tcp
} // namespace elasticJeans

#endif // _ELASTICJEANS_TCP_LINUX_H