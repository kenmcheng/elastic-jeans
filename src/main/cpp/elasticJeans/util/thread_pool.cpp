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

    for (auto& worker : workers_) {
        worker.join();
    }
}

void ThreadPool::add(std::thread&& worker) {
    workers_.emplace_back(std::move(worker));
}

} // namespace elasticJeans
