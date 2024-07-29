#ifndef _LATTICE_THREAD_POOL_H
#define _LATTICE_THREAD_POOL_H

// #include <functional>
// #include <future>
// #include <queue>
#include <thread>
#include <vector>
#include <atomic>

namespace lattice {

class ThreadPool {
public:
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool &operator=(ThreadPool &&) = delete;

    explicit ThreadPool(size_t poolSize = 0);
    virtual ~ThreadPool();

    void add(std::thread&& worker);

    void terminate();

    // void add(std::function<void()> command);

private:
    std::atomic<bool> stop_ = false;
    size_t poolSize_;
    std::vector<std::thread> workers_;
    // std::queue<std::function<void()>> tasks_;

    // std::mutex mtx_;
    // std::condition_variable cv_;

    // void init(size_t poolSize);
};

} // namespace lattice

#endif