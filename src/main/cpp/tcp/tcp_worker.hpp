#ifndef _ELASTICJEANS_TCP_WORKER_H
#define _ELASTICJEANS_TCP_WORKER_H

namespace elasticJeans {
namespace tcp {

class Worker {

};

class WorkerPool {

public:
    WorkerPool(int poolSize = 32);

private:
    int poolSize_;
};

} // namespace tcp
} // namespace elasticJeans

#endif // _ELASTICJEANS_TCP_WORKER_H