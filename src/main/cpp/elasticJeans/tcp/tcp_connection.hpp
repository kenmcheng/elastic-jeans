#ifndef _ELASTICJEANS_TCP_CONNECTION_H
#define _ELASTICJEANS_TCP_CONNECTION_H

#include <string>

namespace elasticJeans {
namespace tcp {

class Connection {
public:
    Connection(const Connection &) = delete;
    Connection &operator=(const Connection &) = delete;

    Connection() = default;

    Connection(int socket_fd, std::string initIP, int initPort, std::string recIP, int recPort);

    ~Connection();

    void setAutoClose(bool autoClose)  { this->autoClose_ = autoClose; }

    bool getAutoClose() { return autoClose_; }

    std::string receiveData(int bufferSize = 0x800);

    void sendData(const std::string& date);

    void fin();

private:
    bool closed_ = false;
    bool autoClose_ = true;
    int conn_socket_fd_;
    std::string initiatorIP_;
    int initiatorPort_;
    std::string receiverIP_;
    int receiverPort_;
};

} // namespace tcp
} // namespace elasticJeans

#endif // _ELASTICJEANS_TCP_CONNECTION_H