#ifndef _LATTICE_THREAD_POOL_EXEC_H
#define _LATTICE_THREAD_POOL_EXEC_H

#include "thread_pool.hpp"
#include "task_queue.hpp"

#include <atomic>

namespace lattice {

class ThreadPoolExecutor {
public:
    ThreadPoolExecutor(const ThreadPoolExecutor &) = delete;
    ThreadPoolExecutor(ThreadPoolExecutor &&) = delete;
    ThreadPoolExecutor &operator=(const ThreadPoolExecutor &) = delete;
    ThreadPoolExecutor &operator=(ThreadPoolExecutor &&) = delete;

    ThreadPoolExecutor(size_t maxWorkers = 0x20, long long maxQueueSize = __LONG_LONG_MAX__, bool autoStart = true);

    ~ThreadPoolExecutor();

    void start();

    void stop();

    template <class F, class... Args>
    std::future<std::result_of_t<F(Args ...)>> submit(F&& f, Args&&... args);

private:
    std::atomic<bool> init_ = false;
    bool stop_ = false;
    size_t maxWorkers_;
    std::mutex mtx_;
    std::unique_ptr<ThreadPool> threadPool_;
    std::unique_ptr<TaskQueue> tasks_;

    void init();
};

template <typename F, typename... Args>
std::future<std::result_of_t<F(Args ...)>> ThreadPoolExecutor::submit(F&& f, Args&&... args) {
    if (stop_)
        throw std::runtime_error("Thread pool executor has been stopped.");
    return tasks_->add(std::forward<F>(f), std::forward<Args>(args)...);
}

} // namespace lattice

#endif // _LATTICE_THREAD_POOL_EXEC_H