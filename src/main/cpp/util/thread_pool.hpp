#ifndef _ELASTICJEANS_THREAD_POOL_H
#define _ELASTICJEANS_THREAD_POOL_H

#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <vector>

namespace elasticJeans {

class ThreadPool {
public:
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool &operator=(ThreadPool &&) = delete;

    ThreadPool(size_t poolSize);
    virtual ~ThreadPool();

    void init();

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;

    std::mutex mtx_;
    std::condition_variable cv_;
    bool stop_;
};


} // namespace elasticJeans

#endif