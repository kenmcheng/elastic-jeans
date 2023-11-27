#include "rest_api_registry.hpp"

namespace elasticJeans {

RestApiRegistry::RestApiRegistry() {
    depotPtr_ = std::make_unique<api_map>();
}

RestApiRegistry& RestApiRegistry::getInstance() {
    static RestApiRegistry theRegistry;
    return theRegistry;
}

std::optional<std::shared_ptr<RestBaseAPI>> RestApiRegistry::lookFor(const http::Method& method, const std::string& path) {
    return lookFor(method.value(), path);
}

std::optional<std::shared_ptr<RestBaseAPI>>  RestApiRegistry::lookFor(http::Method::MethodEnum method, const std::string& path) {
    if (depotPtr_->count(method) && depotPtr_->at(method).count(path))
        return (*depotPtr_)[method][path];

    return std::nullopt;
}

} // namespace elasticJeans