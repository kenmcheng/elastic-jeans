#ifndef _ELASTICJEANS_APPLICATION_IMPL_H
#define _ELASTICJEANS_APPLICATION_IMPL_H

#include <elasticJeans/app.hpp>
#include <elasticJeans/http/http_linux.hpp>

#include <mutex>
#include <condition_variable>

namespace elasticJeans {

class AppImpl : public App {
    
public:
    AppImpl();
    ~AppImpl() override;

private:
    bool stop_ = false;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::unique_ptr<http::HttpServer> serverPtr_;

    void start() override;

    void stop() override;

    App& http(const std::string& ip, int port) override;

    App& https(const std::string& ip, int port) override;
};

} // namespace elasticJeans

#endif // _ELASTICJEANS_APPLICATION_IMPL_H