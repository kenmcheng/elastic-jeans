#ifndef _ELASTICJEANS_TLS_HANDSHAKER_H
#define _ELASTICJEANS_TLS_HANDSHAKER_H

#include <string>

namespace elasticJeans {
namespace tls {

class Handshaker {
public:
    void sayHello();

    void hello(const std::string& clientHello) { hello(clientHello.c_str(), clientHello.size()); }

    void hello(const char clientHello[], size_t sz);

private:


};

} // namespace tls
} // namespace elasticJeans

#endif