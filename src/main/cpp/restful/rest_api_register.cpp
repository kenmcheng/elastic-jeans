#include "rest_api_register.hpp"

namespace elasticJeans {

RestApiRegister::RestApiRegister() {
    depotPtr_ = std::make_unique<api_map>();
}

std::optional<std::shared_ptr<RestBaseAPI>> RestApiRegister::lookFor(const http::Method& method, const std::string& path) {
    return lookFor(method.value(), path);
}

std::optional<std::shared_ptr<RestBaseAPI>>  RestApiRegister::lookFor(http::Method::MethodEnum method, const std::string& path) {
    if (depotPtr_->count(method) && depotPtr_->at(method).count(path))
        return (*depotPtr_)[method][path];

    return std::nullopt;
}

} // namespace elasticJeans