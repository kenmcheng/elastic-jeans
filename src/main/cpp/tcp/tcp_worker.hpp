#ifndef _ELASTICJEANS_TCP_WORKER_H
#define _ELASTICJEANS_TCP_WORKER_H

#include "../util/thread_pool_exec.hpp"

#include <queue>
#include <memory>

namespace elasticJeans {

using cbFn = std::function<int(std::string&,  std::string&)>;
using uniqueCbFn = std::unique_ptr<cbFn>;

namespace tcp {

class Workers {
public:
    explicit Workers(size_t poolSize = 32) :
        poolSize_(poolSize) {
        threadPoolExecutor_ = std::make_unique<ThreadPoolExecutor>(poolSize);
    }

    template<typename F>
    int registerCbFunc(F&& f);

    void handle(int conn_socket_fd);

    void sendResponse(int conn_socket_fd, const std::string& response);

private:
    size_t poolSize_;
    int bufferSize_ = 0X200;
    std::vector<uniqueCbFn> fnChain;
    std::unique_ptr<ThreadPoolExecutor> threadPoolExecutor_;

    void doHandle(int conn_socket_fd);

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