#ifndef _ELASTICJEANS_TCP_WORKER_H
#define _ELASTICJEANS_TCP_WORKER_H
#include <elasticJeans/tcp/tcp_connection.hpp>
#include <elasticJeans/util/thread_pool_exec.hpp>

#include <queue>
#include <memory>
#include <arpa/inet.h>

#define DEFAULT_WORKER_SIZE 32

namespace elasticJeans {
namespace tcp {

class Workers {
public:
    Workers(size_t poolSize = DEFAULT_WORKER_SIZE) :
        poolSize_{poolSize} {
        threadPoolExecutor_ = std::make_unique<ThreadPoolExecutor>(poolSize, __LONG_LONG_MAX__, false);
    }

    int run();

    template<typename F>
    void handle(F&& f);

private:
    size_t poolSize_;
    int bufferSize_ = 0X200;
    std::unique_ptr<ThreadPoolExecutor> threadPoolExecutor_;

};

template<typename F>
void Workers::handle(F&& f) {
    threadPoolExecutor_->submit(std::forward<F>(f));
}

} // namespace tcp
} // namespace elasticJeans

#endif // _ELASTICJEANS_TCP_WORKER_H