#ifndef _ELASTICJEANS_APPLICATION_H
#define _ELASTICJEANS_APPLICATION_H

#include <string>
#include <vector>

namespace elasticJeans {

class App {

public:
    class Initiator {
    public:
        Initiator();

        Initiator& http(const std::string& ip, int port);

        Initiator& https(const std::string& ip, int port);

        void start();
    };

    virtual ~App() {}

    static Initiator init() { return Initiator(); }

private:
    virtual void start() = 0;

    virtual void stop() = 0;

    virtual App& http(const std::string& ip, int port) = 0;

    virtual App& https(const std::string& ip, int port) = 0;

};

void sigFunc(int signal);

App::Initiator app();

App::Initiator app(const std::string& ip, int port, bool withTLS = false);

} // namespace elasticJeans

#endif