#ifndef _ELASTICJEANS_SECURE_CONNECTION_H
#define _ELASTICJEANS_SECURE_CONNECTION_H

#include <elasticJeans/tcp/tcp_connection.hpp>

#include <openssl/ssl.h>
#include <memory>

namespace elasticJeans {
namespace tls {

struct SSLDel {
    void operator()(SSL* ssl) { SSL_shutdown(ssl); SSL_free(ssl); }
};

class SecureConnection : public tcp::Connection {
    

public:
    SecureConnection(
        int socket_fd, 
        std::string initIP, 
        int initPort, 
        std::string recIP, 
        int recPort,
        std::unique_ptr<SSL, SSLDel> ssl
        );

    ~SecureConnection() = default;

    std::string receiveData(ssize_t bufferSize = 0x800) override;

    void sendData(const std::string& date) override;


protected:
    std::unique_ptr<SSL, SSLDel> ssl_;

};

} // tls
} // elasticJeans

#endif // _ELASTICJEANS_SECURE_CONNECTION_H
