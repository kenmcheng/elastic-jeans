#include "task_queue.hpp"


namespace elasticJeans {

void TaskQueue::add(std::function<void()>& fn) {
    {
        tasks_.emplace(fn);
    }
    
    // cv_.notify_one();
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