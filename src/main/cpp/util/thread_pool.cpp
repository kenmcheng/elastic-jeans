#include "thread_pool.hpp"

#include <iostream>

namespace elasticJeans {

ThreadPool::~ThreadPool() {
    std::unique_lock<std::mutex> lock(mtx_);
    stop_ = true;

    cv_.notify_all();

    for (auto& worker : workers_) {
        worker.join();
    }
}

} // namespace elasticJeans