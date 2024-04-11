#ifndef _ELASTICJEANS_TLS_HANDSHAKER_H
#define _ELASTICJEANS_TLS_HANDSHAKER_H

#include <elasticJeans/tcp/tcp_handler.hpp>

#include <openssl/ssl.h> /* core library */

#include <string>

namespace elasticJeans {
namespace tls {

struct SSLCtxDel {
    void operator()(SSL_CTX* ctx) { SSL_CTX_free(ctx); }
};

class TcpListener;

class Handshaker: public tcp::TcpHandler {
public:

    Handshaker();

    ~Handshaker();

    int init();

    void doHandle(int conn_socket_fd, const sockaddr_in& scaddr) override;

    void setCertificateFile(std::string certificateFile) { this->certificateFile_ = certificateFile; }

    std::string getCertificateFile() { return this->certificateFile_; }

    void setPrivateKeyFile(std::string privateKeyFile) { this->privateKeyFile_ = privateKeyFile; }

    std::string getPrivateKeyFile() { return this->privateKeyFile_; }

private:
    std::unique_ptr<SSL_CTX, SSLCtxDel> ctx_;
    std::string certificateFile_;
    std::string privateKeyFile_;

    void initSSL();

    void cleanup();

    void printHello(const char clientHello[], size_t sz);
    
};

} // namespace tls
} // namespace elasticJeans

#endif