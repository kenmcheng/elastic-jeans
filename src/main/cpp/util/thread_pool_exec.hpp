#ifndef _ELASTICJEANS_THREAD_POOL_EXEC_H
#define _ELASTICJEANS_THREAD_POOL_EXEC_H

#include "thread_pool.hpp"
#include "task_queue.hpp"

namespace elasticJeans {

class ThreadPoolExecutor {
public:
    ThreadPoolExecutor(ssize_t maxWorkers = 0x20, long long maxQueueSize = __LONG_LONG_MAX__) :
        threadPool_(maxWorkers), tasks_(maxQueueSize), stop_(false) {}

    template <class F, class... ARGS>
    std::future<std::result_of_t<F(ARGS ...)>> add(F&& f, ARGS&&... args);

    
    template <typename F, typename... ARGS>
    std::future<std::result_of_t<F(ARGS ...)>> handle(F&& f, ARGS&&... args);

private:
    ThreadPool threadPool_;
    TaskQueue tasks_;
    std::mutex mtx_;
    bool stop_;
};

template <typename F, typename... ARGS>
std::future<std::result_of_t<F(ARGS ...)>> ThreadPoolExecutor::add(F&& f, ARGS&&... args) {
    using return_type = std::result_of_t<F(ARGS...)>;

    auto task = std::make_shared<std::packaged_task<return_type()>> (
        std::bind(std::forward<F>(f), std::forward<ARGS>(args)...)
    );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock lock(mtx_);
        if (stop_)
            throw std::runtime_error("Thread pool has been stopped.");
        
        tasks_.add([task]() -> void { task(); });
    }
    // cv_.notify_one();
    return res;
}

} // namespace elasticJeans

#endif // _ELASTICJEANS_THREAD_POOL_EXEC_H