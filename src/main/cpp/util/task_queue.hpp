#ifndef _ELASTICJEANS_Task_QUEUE_H
#define _ELASTICJEANS_Task_QUEUE_H

#include <functional>
#include <future>
#include <queue>

#include "thread_pool.hpp"

namespace elasticJeans {

class TaskQueue {
public:
    explicit TaskQueue(long long maxQueueSize = __LONG_LONG_MAX__) : maxQueueSize_(maxQueueSize) {};

    void add(std::function<void()>& fn);

    // void add(std::function<void()>&& fn);

    std::function<void()> next();

private:
    std::queue<std::function<void()>> tasks_;
    long long maxQueueSize_;
    std::mutex mtx_;
};

} // namespace elasticJeans

#endif // _ELASTICJEANS_Task_QUEUE_H