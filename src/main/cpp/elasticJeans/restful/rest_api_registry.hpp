#ifndef _ELASTICJEANS_REST_API_REGISTRY_H
#define _ELASTICJEANS_REST_API_REGISTRY_H

#include "rest.hpp"

#include <memory>
#include <optional>

namespace elasticJeans {

class RestApiRegistry {
    using api_map = std::unordered_map<http::Method::MethodEnum, 
                        std::unordered_map<std::string, std::shared_ptr<RestBaseAPI>>>;
public:
    RestApiRegistry();

    static RestApiRegistry& getInstance();

    template<http::Method::MethodEnum M ,typename F>
    void add(const RestAPI<M, F>& restApi) {
        this->add<M, F>(restApi, restApi.getPath());
    }

    template<http::Method::MethodEnum M ,typename F>
    void add(const RestAPI<M, F>& restApi, const std::string& path) {
        (*depotPtr_)[M][path] = std::make_shared<RestAPI<M, F>>(restApi);
    }

    std::optional<std::shared_ptr<RestBaseAPI>> lookFor(const http::Method& method, const std::string& path);

    std::optional<std::shared_ptr<RestBaseAPI>> lookFor(http::Method::MethodEnum method, const std::string& path);

private:
    std::unique_ptr<api_map> depotPtr_;

};

template<http::Method::MethodEnum M, typename F>
RestAPI<M, F> Rest(const std::string& path, F&& f) {
    auto restApi = RestAPI<M, F>(path, std::forward<F>(f));
    RestApiRegistry::getInstance().add(restApi);
    return restApi;
}

} // namespace elasticJeans

#endif