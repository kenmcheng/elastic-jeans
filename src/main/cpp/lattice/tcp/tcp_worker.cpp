#include <lattice/tcp/tcp_worker.hpp>
#include <lattice/tcp/tcp_linux.hpp>
#include <lattice/log/log.hpp>

#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>
#include <exception>


namespace lattice {
namespace tcp {

int Workers::run() {
    threadPoolExecutor_->start();

    return 0;
}


} // namespace tcp
} // namespace lattice