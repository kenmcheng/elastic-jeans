#ifndef _ELASTICJEANS_APPLICATION_H
#define _ELASTICJEANS_APPLICATION_H

#include <http/http_linux.hpp>
#include <restful/rest.hpp>

#include <vector>
#include <csignal>
#include <memory>

using elasticJeans::http::Method;

namespace elasticJeans {

class App {
    std::vector<std::unique_ptr<http::HttpServer>> serverPtrs_;
public:
    class Initiator {
    public:
        Initiator();

        Initiator& http(const std::string& ip, int port);

        Initiator& https(const std::string& ip, int port);

        void start();
    };

    static Initiator init() { return Initiator(); }

private:
    void start();

    App& http(const std::string& ip, int port);

    App& https(const std::string& ip, int port);

};

// extern std::unique_ptr<http::HttpServer> serverPtr;
// extern std::unique_ptr<App> appPtr;

void sigFunc(int signal);

inline App::Initiator app() {
    std::signal(SIGINT, sigFunc);
    return App::init();
}

inline App::Initiator app(const std::string& ip, int port, bool withTLS = false) {
    App::Initiator newApp = app();
    if (withTLS)
        newApp.https(ip, port);
    else
        newApp.http(ip, port);
    return newApp;
}

// inline void start() {
//     serverPtr->start();
// }


} // namespace elasticJeans

#endif