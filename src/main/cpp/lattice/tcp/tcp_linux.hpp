#ifndef _LATTICE_TCP_LINUX_H
#define _LATTICE_TCP_LINUX_H

#include <lattice/tcp/tcp_handler.hpp>
#include <lattice/tcp/tcp_worker.hpp>
#include <lattice/tls/handshaker.hpp>

#include <string>
#include <type_traits>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <atomic>

namespace lattice {

namespace tcp {

class TcpListener {
    friend class DefaultTcpHandler;
    friend class tls::Handshaker;

public:

    TcpListener(
        std::string ipAddress = "127.0.0.1", 
        int port = 8080,
        std::shared_ptr<Workers> workers = std::make_shared<Workers>(),
        std::unique_ptr<TcpHandler> handler = std::make_unique<DefaultTcpHandler>());

    ~TcpListener();

    TcpListener(TcpListener&) = delete;
    TcpListener& operator=(TcpListener&) = delete;

    TcpListener(TcpListener&&) = default;
    TcpListener& operator=(TcpListener&&) = default;

    int init();
    void start();
    void stop();

    /**
     * Register callback functions which will be performed in incoming requsets.
    */
    template<typename F>
    int registerCbFunc(F&& f);

private:
    std::atomic<bool> init_ = false;
    bool stop_ = false;
    std::string ipv4Address_;
    int port_;
    int socket_ipv4_fd_;
    int socketQueueSize_ = 30;
    struct sockaddr_in socketAddress_;
    unsigned int socketAddress_len_;
    
    std::unique_ptr<TcpHandler> handler_;
    std::shared_ptr<Workers> workers_;

    void _listen_ipv4();
    int _accept_ipv4() noexcept;
};


template<typename F>
int TcpListener::registerCbFunc(F&& f) {
    return handler_->registerCbFunc(std::forward<F>(f));
}

} // namespace tcp
} // namespace lattice

#endif // _LATTICE_TCP_LINUX_H