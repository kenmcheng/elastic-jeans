#include "thread_pool.hpp"

#include <iostream>

namespace elasticJeans {

ThreadPool::ThreadPool(size_t poolSize) : poolSize_(poolSize), stop_(false) {
    // this->init(poolSize);
}

ThreadPool::~ThreadPool() {
    this->terminate();
}

void ThreadPool::terminate() {
    {
        // std::unique_lock<std::mutex> lock(mtx_);
        if (stop_) return;
        stop_ = true;
    }

    // cv_.notify_all();

    for (auto& worker : workers_) {
        worker.join();
    }
}

// void ThreadPool::init(size_t poolSize) {
//     for (size_t i =  0; i < poolSize; i++) {
//         std::thread worker([this]() {
//             while (true) {
//                 std::function<void()> task;
//                 {
//                     std::unique_lock lock(mtx_);
//                     cv_.wait(lock, [this]() { return stop_ || !tasks_.empty(); });
//                     if (stop_ && tasks_.empty()) return;

//                     task = std::move(tasks_.front());
//                     tasks_.pop();
//                 }
//                 task();
//             }
//         });
//         add(std::move(worker));
//     }
// }

void ThreadPool::add(std::thread&& worker) {
    workers_.emplace_back(std::move(worker));
}

} // namespace elasticJeans
