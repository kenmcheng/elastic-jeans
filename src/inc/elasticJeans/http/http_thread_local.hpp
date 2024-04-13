#ifndef _ELASTICJEANS_HTTP_THREAD_LOCAL_H
#define _ELASTICJEANS_HTTP_THREAD_LOCAL_H

#include <elasticJeans/http/http_request.hpp>
#include <elasticJeans/http/http_response.hpp>

#include <memory>

namespace elasticJeans {
namespace http {

extern thread_local std::unique_ptr<HttpRequest> request;
extern thread_local std::unique_ptr<HttpResponse> response;

} // namespace http
} // namespace elasticJeans

#endif // _ELASTICJEANS_HTTP_THREAD_LOCAL_H