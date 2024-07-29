#include "handshaker.hpp"
#include <lattice/tls/secure_connection.hpp>
#include <lattice/tcp/tcp_linux.hpp>
#include <lattice/log/log.hpp>

#include <stdio.h>
#include <stdlib.h>

#include <openssl/bio.h> /* BasicInput/Output streams */
#include <openssl/err.h> /* errors */
// #include <openssl/ssl.h> /* core library */

#include <memory>

#include <iostream>

#define BuffSize 1024

namespace lattice {
namespace tls {

Handshaker::Handshaker(): tcp::TcpHandler{} {

}

Handshaker::~Handshaker() {
    cleanup();
}

int Handshaker::init() {
    #if OPENSSL_VERSION_NUMBER < 0x10100000L
    initSSL();
    #endif

    ctx_ = std::unique_ptr<SSL_CTX, SSLCtxDel>(SSL_CTX_new(TLS_method()));
    SSL_CTX_set_min_proto_version(ctx_.get(), TLS1_2_VERSION);

    if (SSL_CTX_use_certificate_file(ctx_.get(), certificateFile_.c_str(), SSL_FILETYPE_PEM) > 0 &&
        SSL_CTX_use_PrivateKey_file(ctx_.get(), privateKeyFile_.c_str(), SSL_FILETYPE_PEM) > 0) {
        Log::info("SSL cert set!");
        init_ = true;
        return 0;
    }

    Log::warn("SSL cert cannot be set");
    return -1;
}

void Handshaker::doHandle(int conn_socket_fd, const sockaddr_in& scaddr) {
    try {
        std::unique_ptr<SSL, SSLDel> ssl(SSL_new(ctx_.get()));
        SSL_set_fd(ssl.get(), conn_socket_fd);

        if (SSL_accept(ssl.get()) <= 0) {
            Log::error("Invalid TLS request");
            close(conn_socket_fd);
            return;
        }
        
        SecureConnection conn{
            conn_socket_fd,
            std::string{inet_ntoa(scaddr.sin_addr)},
            (int) ntohs(scaddr.sin_port), 
            tcpListener_->ipv4Address_, 
            tcpListener_->port_,
            move(ssl)
        };

        runCbFunc(conn);

    } catch (...) {
        Log::error("Failed to handle reuqest");
        close(conn_socket_fd);
    }

}

void Handshaker::printHello(const char clientHello[], size_t sz) {
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

void Handshaker::initSSL() { 
    SSL_load_error_strings(); 
    SSL_library_init(); 
}

void Handshaker::cleanup() { 
    SSL_CTX_free(ctx_.get()); 
}

} // namespace tls
} // namespace lattice
