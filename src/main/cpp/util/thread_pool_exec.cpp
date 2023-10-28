#include "thread_pool_exec.hpp"
#include "../log/log.hpp"


namespace elasticJeans {

ThreadPoolExecutor::ThreadPoolExecutor(size_t maxWorkers, long long maxQueueSize) :
    maxWorkers_{maxWorkers}, stop_{false} {
    tasks_ = std::make_unique<TaskQueue>(maxQueueSize);
    threadPool_ = std::make_unique<ThreadPool>();
    this->init();
}

ThreadPoolExecutor::~ThreadPoolExecutor() {
    stop();
}

void ThreadPoolExecutor::stop() {
    {
        // std::unique_lock lock = tasks_->lock();
        Log::info("stopping thread pool executor...");
        stop_ = true;
    }
    Log::info("stopping task queue...");
    tasks_->terminate();
    Log::info("stopping thread pool...");
    threadPool_->terminate();
}

void ThreadPoolExecutor::init() {
    for (size_t i =  0; i < maxWorkers_; i++) {
        std::thread worker([this]() {
            while (true) {
                if (stop_ && tasks_->empty()) break;
                std::function<void()> task = tasks_->fetch();
                // {
                //     std::unique_lock lock(mtx_);
                //     cv_.wait(lock, [this]() { return stop_ || !tasks_.empty(); });
                //     if (stop_ && tasks_.empty()) return;

                //     task = std::move(tasks_.front());
                //     tasks_.pop();
                // }
                task();
            }
        });
        threadPool_->add(std::move(worker));
    }
}

} // namespace elasticJeans