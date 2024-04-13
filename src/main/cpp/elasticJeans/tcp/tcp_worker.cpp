#include <elasticJeans/tcp/tcp_worker.hpp>
#include <elasticJeans/tcp/tcp_linux.hpp>
#include <elasticJeans/log/log.hpp>

#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>
#include <exception>


namespace elasticJeans {
namespace tcp {

int Workers::run() {
    threadPoolExecutor_->start();

    return 0;
}


} // namespace tcp
} // namespace elasticJeans