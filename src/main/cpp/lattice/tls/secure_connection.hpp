#ifndef _LATTICE_SECURE_CONNECTION_H
#define _LATTICE_SECURE_CONNECTION_H

#include <lattice/tcp/tcp_connection.hpp>

#include <openssl/ssl.h>
#include <memory>

namespace lattice {
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
} // lattice

#endif // _LATTICE_SECURE_CONNECTION_H
