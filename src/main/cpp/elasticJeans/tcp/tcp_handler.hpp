#ifndef _ELASTICJEANS_TCP_HANDLER_H
#define _ELASTICJEANS_TCP_HANDLER_H

#include <elasticJeans/tcp/tcp_connection.hpp>

#include <vector>
#include <memory>
#include <functional>
#include <arpa/inet.h>
#include <atomic>

#define PROCEED 0
#define INTERRUPT_WO_CLOSE 1
#define INTERRUPT_W_CLOSE 2

namespace elasticJeans {
namespace tcp {

using cbFn = std::function<int(Connection&)>;
using uniqueCbFn = std::unique_ptr<cbFn>;

class TcpListener;

class TcpHandler {
public:

    TcpHandler() = default;

    TcpHandler(TcpListener* tcpListener): tcpListener_{tcpListener} {}

    virtual void doHandle(int conn_socket_fd, const sockaddr_in& scaddr) = 0;

    template<typename F>
    int registerCbFunc(F&& f);

    void setTcpListener(TcpListener* tcpListener) { this->tcpListener_ = tcpListener; }

    TcpListener* getTcpListener() { return this->tcpListener_; }

    bool isInit() { return init_; }

protected:
    std::atomic<bool> init_ = false;
    std::vector<uniqueCbFn> fnChain;
    TcpListener* tcpListener_;

    virtual int init() { init_ = true; return 0; }

    virtual void runCbFunc(Connection& conn);
};

class DefaultTcpHandler : public TcpHandler {
public:
    DefaultTcpHandler() = default;
    DefaultTcpHandler(TcpListener* tcpListener): TcpHandler{tcpListener} {
    }

    void doHandle(int conn_socket_fd, const sockaddr_in& scaddr) override;

private:

};

template<typename F>
int TcpHandler::registerCbFunc(F&& f) {

    fnChain.push_back(
        std::make_unique<cbFn>(std::forward<F>(f)));

    return fnChain.size();
}

} // tcp
} // elasticJeans
#endif // _ELASTICJEANS_TCP_HANDLER_H