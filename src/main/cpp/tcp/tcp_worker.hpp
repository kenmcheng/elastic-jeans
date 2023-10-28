#ifndef _ELASTICJEANS_TCP_WORKER_H
#define _ELASTICJEANS_TCP_WORKER_H
#include "tcp_connection.hpp"
#include "../util/thread_pool_exec.hpp"

#include <queue>
#include <memory>
#include <arpa/inet.h>

namespace elasticJeans {
namespace tcp {

using cbFn = std::function<int(Connection&)>;
using uniqueCbFn = std::unique_ptr<cbFn>;

class TcpListener;

class Workers {
public:
    Workers(TcpListener* tcpListener, size_t poolSize = 32) :
        tcpListener_{tcpListener}, poolSize_{poolSize} {
        threadPoolExecutor_ = std::make_unique<ThreadPoolExecutor>(poolSize);
    }

    template<typename F>
    int registerCbFunc(F&& f);

    void handle(int conn_socket_fd, const sockaddr_in& scaddr);

private:
    const TcpListener* tcpListener_;
    size_t poolSize_;
    int bufferSize_ = 0X200;
    std::vector<uniqueCbFn> fnChain;
    std::unique_ptr<ThreadPoolExecutor> threadPoolExecutor_;

    void doHandle(int conn_socket_fd, const sockaddr_in& scaddr);

};

template<typename F>
int Workers::registerCbFunc(F&& f) {

    fnChain.push_back(
        std::make_unique<cbFn>(std::forward<F>(f)));

    return fnChain.size();
}

} // namespace tcp
} // namespace elasticJeans

#endif // _ELASTICJEANS_TCP_WORKER_H