#ifndef _LATTICE_TCP_HANDLER_H
#define _LATTICE_TCP_HANDLER_H

#include <lattice/tcp/tcp_connection.hpp>

#include <vector>
#include <memory>
#include <functional>
#include <arpa/inet.h>
#include <atomic>

#define PROCEED 0
#define INTERRUPT_WO_CLOSE 1
#define INTERRUPT_W_CLOSE 2

namespace lattice {
namespace tcp {

using cbFn = std::function<int(Connection&)>;
using uniqueCbFn = std::unique_ptr<cbFn>;

class TcpListener;

class TcpHandler {
public:

    TcpHandler(): TcpHandler{nullptr} {}

    TcpHandler(TcpListener* tcpListener): tcpListener_{tcpListener} { init(); }

    virtual void doHandle(int conn_socket_fd, const sockaddr_in& scaddr) = 0;

    template<typename F>
    int registerCbFunc(F&& f);

    void setTcpListener(TcpListener* tcpListener) { this->tcpListener_ = tcpListener; }

    TcpListener* getTcpListener() { return this->tcpListener_; }

    bool isInit() { return init_; }

protected:
    std::atomic<bool> init_ = false;
    std::vector<uniqueCbFn> fnChain_;
    TcpListener* tcpListener_;

    virtual int init() { init_ = true; return 0; }

    virtual void runCbFunc(Connection& conn);
};

class DefaultTcpHandler : public TcpHandler {
public:
    DefaultTcpHandler(): DefaultTcpHandler{nullptr} {}
    DefaultTcpHandler(TcpListener* tcpListener): TcpHandler{tcpListener} {
    }

    void doHandle(int conn_socket_fd, const sockaddr_in& scaddr) override;

private:

};

template<typename F>
int TcpHandler::registerCbFunc(F&& f) {

    fnChain_.push_back(
        std::make_unique<cbFn>(std::forward<F>(f)));

    return fnChain_.size();
}

} // tcp
} // lattice
#endif // _LATTICE_TCP_HANDLER_H