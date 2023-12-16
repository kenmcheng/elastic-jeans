#include "task_queue.hpp"
#include <elasticJeans/log/log.hpp>


namespace elasticJeans {


TaskQueue::~TaskQueue() {
    this->terminate();
}

void TaskQueue::terminate() {
    {
        std::unique_lock lock = this->lock();
        if (terminate_) return;
        terminate_ = true;
    }
    // Dequeue all the task first
    while (!tasks_.empty()) {
        cv_.notify_one();
    }
    cv_.notify_all();
}

void TaskQueue::add(std::function<void()>& fn) {
    {
        // std::unique_lock lock(mtx_);
        std::unique_lock lock = this->lock();
        tasks_.emplace(fn);
    }
    cv_.notify_one();
}

std::function<void()> TaskQueue::fetch() {
    // std::unique_lock lock(mtx_);
    std::unique_lock lock = this->lock();
    cv_.wait(lock, [this]() { return terminate_ || !tasks_.empty(); });
    // if (tasks_.empty()) return;
    return next();
}

std::function<void()> TaskQueue::next() {
    if (tasks_.empty()) {
        return []() {;};
    }
    auto fn = std::move(tasks_.front());
    tasks_.pop();
    return fn;
}

} // namespace elasticJeans