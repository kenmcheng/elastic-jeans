#ifndef _ELASTICJEANS_Task_QUEUE_H
#define _ELASTICJEANS_Task_QUEUE_H

#include "thread_pool.hpp"
#include <log/log.hpp>

#include <functional>
#include <future>
#include <queue>

namespace elasticJeans {

class TaskQueue {
public:
    TaskQueue(const TaskQueue &) = delete;
    TaskQueue(TaskQueue &&) = delete;
    TaskQueue &operator=(const TaskQueue &) = delete;
    TaskQueue &operator=(TaskQueue &&) = delete;

    explicit TaskQueue(long long maxQueueSize = __LONG_LONG_MAX__) : maxQueueSize_{maxQueueSize} {};

    ~TaskQueue();

    void terminate();

    void add(std::function<void()>& fn);

    template <typename F, typename... Args>
    std::future<std::result_of_t<F(Args ...)>> add(F&& f, Args&&... args);

    std::function<void()> fetch();

    std::function<void()> next();

    bool empty() { return tasks_.empty(); }

    std::unique_lock<std::mutex> lock() { return std::unique_lock(mtx_); };

private:
    bool terminate_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::queue<std::function<void()>> tasks_;
    long long maxQueueSize_;

};

template <typename F, typename... Args>
std::future<std::result_of_t<F(Args ...)>> TaskQueue::add(F&& f, Args&&... args) {
    using return_type = std::result_of_t<F(Args...)>;

    auto task = std::make_shared<std::packaged_task<return_type()>> (
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock lock(mtx_);
        // if (terminate_)
        //     throw std::runtime_error("Thread pool has been stopped.");
        
        tasks_.emplace([task]() -> void { (*task)(); });
    }
    cv_.notify_one();
    return res;
}

} // namespace elasticJeans

#endif // _ELASTICJEANS_Task_QUEUE_H