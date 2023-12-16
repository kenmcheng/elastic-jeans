#ifndef _ELASTICJEANS_APPLICATION_IMPL_H
#define _ELASTICJEANS_APPLICATION_IMPL_H

#include <elasticJeans/app.hpp>
#include <elasticJeans/http/http_linux.hpp>


namespace elasticJeans {

class AppImpl : public App {
    
public:
    AppImpl() = default;
    ~AppImpl() override = default;

private:
    std::vector<std::unique_ptr<http::HttpServer>> serverPtrs_;

    void start() override;

    App& http(const std::string& ip, int port) override;

    App& https(const std::string& ip, int port) override;
};

} // namespace elasticJeans

#endif // _ELASTICJEANS_APPLICATION_IMPL_H