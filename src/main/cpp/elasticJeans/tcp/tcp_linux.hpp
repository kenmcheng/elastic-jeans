#ifndef _ELASTICJEANS_TCP_LINUX_H
#define _ELASTICJEANS_TCP_LINUX_H

#include <elasticJeans/tcp/tcp_worker.hpp>

#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>


namespace elasticJeans {

namespace tcp {

class TcpListener {
    friend class Workers;

public:
    TcpListener(std::string ipAddress = "127.0.0.1", int port = 8080, int workerPoolSize = 32);
    ~TcpListener();

    int init();
    void start();
    void stop();

    template<typename F>
    int registerCbFunc(F&& f);

private:
    std::string ipv4Address_;
    int port_;
    int socket_ipv4_fd_;
    int socketQueueSize_ = 30;
    struct sockaddr_in socketAddress_;
    unsigned int socketAddress_len_;
    bool stop_ = false;
    std::unique_ptr<Workers> workers_;

    void _listen_ipv4();
    int _accept_ipv4();
};


template<typename F>
int TcpListener::registerCbFunc(F&& f) {
    return workers_->registerCbFunc(std::forward<F>(f));
}

} // namespace tcp
} // namespace elasticJeans

#endif // _ELASTICJEANS_TCP_LINUX_H