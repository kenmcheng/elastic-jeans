#include "handshaker.hpp"

#include <iostream>

namespace elasticJeans {
namespace tls {

void Handshaker::hello(const char clientHello[], size_t sz) {
    for (size_t i =  0; i < sz; i++) {
        unsigned char c = clientHello[i];
        if (c == '\r') continue;
        if (c == '\n') {
            std::cout << std::endl;
            continue;
        }
        std::cout << (int)c << " " ;
    }
    std::cout << std::endl;
}

} // namespace tls
} // namespace elasticJeans
