#ifndef _ELASTICJEANS_APPLICATION_H
#define _ELASTICJEANS_APPLICATION_H

#include <string>
#include <vector>

// using elasticJeans::http::Method;

namespace elasticJeans {

class App {
    // std::vector<std::unique_ptr<http::HttpServer>> serverPtrs_;
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

    virtual App& http(const std::string& ip, int port) = 0;

    virtual App& https(const std::string& ip, int port) = 0;

};

// extern std::unique_ptr<http::HttpServer> serverPtr;
// extern std::unique_ptr<App> appPtr;

void sigFunc(int signal);

App::Initiator app();

App::Initiator app(const std::string& ip, int port, bool withTLS = false);

// inline void start() {
//     serverPtr->start();
// }


} // namespace elasticJeans

#endif