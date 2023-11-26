#ifndef _ELASTICJEANS_THREAD_POOL_H
#define _ELASTICJEANS_THREAD_POOL_H

// #include <functional>
// #include <future>
// #include <queue>
#include <thread>
#include <vector>

namespace elasticJeans {

class ThreadPool {
public:
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool &operator=(ThreadPool &&) = delete;

    explicit ThreadPool(size_t poolSize = 0);
    virtual ~ThreadPool();

    // template <typename F, typename... ARGS>
    // std::future<std::result_of_t<F(ARGS ...)>> add(F&& f, ARGS&&... args);

    void add(std::thread&& worker);

    void terminate();

    // void add(std::function<void()> command);

private:
    size_t poolSize_;
    std::vector<std::thread> workers_;
    // std::queue<std::function<void()>> tasks_;

    // std::mutex mtx_;
    // std::condition_variable cv_;
    bool stop_;

    // void init(size_t poolSize);
};

// template <typename F, typename... ARGS>
// std::future<std::result_of_t<F(ARGS ...)>> ThreadPool::add(F&& f, ARGS&&... args) {
//     using return_type = std::result_of_t<F(ARGS...)>;

//     auto task = std::make_shared<std::packaged_task<return_type()>> (
//         std::bind(std::forward<F>(f), std::forward<ARGS>(args)...)
//     );

//     std::future<return_type> res = task->get_future();
//     {
//         std::unique_lock lock(mtx_);
//         if (stop_)
//             throw std::runtime_error("Thread pool has been stopped.");
        
//         tasks_.emplace([task]() -> void { (*task)(); });
//     }
//     cv_.notify_one();
//     return res;
// }


} // namespace elasticJeans

#endif