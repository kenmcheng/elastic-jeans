#ifndef _ELASTICJEANS_REST_API_REGISTER_H
#define _ELASTICJEANS_REST_API_REGISTER_H

#include "rest.hpp"

#include <memory>
#include <optional>

namespace elasticJeans {

class RestApiRegister {
    using api_map = std::unordered_map<http::Method::MethodEnum, 
                        std::unordered_map<std::string, std::shared_ptr<RestBaseAPI>>>;
public:
    RestApiRegister();

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

} // namespace elasticJeans

#endif