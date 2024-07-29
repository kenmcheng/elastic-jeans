#ifndef _LATTICE_HTTP_THREAD_LOCAL_H
#define _LATTICE_HTTP_THREAD_LOCAL_H

#include <lattice/http/http_request.hpp>
#include <lattice/http/http_response.hpp>

#include <memory>

namespace lattice {
namespace http {

extern thread_local std::unique_ptr<HttpRequest> request;
extern thread_local std::unique_ptr<HttpResponse> response;

} // namespace http
} // namespace lattice

#endif // _LATTICE_HTTP_THREAD_LOCAL_H