#ifndef _ELASTICJEANS_TCP_LINUX_H
#define _ELASTICJEANS_TCP_LINUX_H

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "tcp_worker.hpp"

namespace elasticJeans {

using cbFuncType = std::function<void(std::string&,  std::string&)>;
namespace tcp {

class TcpListener {

public:
    TcpListener(std::string ipAddress = "127.0.0.1", int port = 8080, int workerPoolSize = 32);
    ~TcpListener();

    int init();
    void start();
    void stop();

    template<typename F>
    int registerCbFunc(F&& f);

private:
    std::string ipAddress_;
    int port_;
    int socket_fd_;
    struct sockaddr_in socketAddress_;
    unsigned int socketAddress_len_;
    bool stop_ = false;
    Workers workers_;

    void _listen();
    int _accept();
};


template<typename F>
int TcpListener::registerCbFunc(F&& f) {
    return workers_.registerCbFunc(std::forward<F>(f));
}

} // namespace tcp
} // namespace elasticJeans

#endif // _ELASTICJEANS_TCP_LINUX_H