#ifndef _LATTICE_REST_H
#define _LATTICE_REST_H

#include <lattice/http/http_request.hpp>
#include <lattice/http/http_response.hpp>
#include <lattice/util/serializable.hpp>

#include <functional>
#include <cstdarg>
#include <type_traits>
#include <initializer_list>
#include <sstream>

namespace lattice {

template<typename, typename Enable = void>
struct Fn_type;

template<typename R, typename C, typename... Args>
struct Fn_type<R(C::*)(Args...),
    std::enable_if_t<(... && std::is_convertible_v<Args, Serializable>)>> {
    using type = R(Args...);
    using args_tuple = std::tuple<Args...>;
    using return_type = R;
};

template<typename R, typename C, typename... Args>
struct Fn_type<R(C::*)(Args...) const,
    std::enable_if_t<(... && std::is_convertible_v<Args, Serializable>)>> {
    using type = R(Args...);
    using args_tuple = std::tuple<Args...>;
    using return_type = R;
};

class RestBaseAPI {
public:
    // RestBaseAPI() {}
    virtual ~RestBaseAPI() {}

    RestBaseAPI(const http::Method::MethodEnum& method, const std::string& path) :
        method_(method),
        path_(path) {}

    virtual void invoke() {}
    
    virtual void invoke(Serializable arg) {}

    virtual void invoke(std::initializer_list<Serializable> args) {}

    std::string info() {
        std::ostringstream oss;
        oss << "API info: " << http::Method::METHODS[method_] <<  " " << path_;
        return oss.str();
    }

protected:
    http::Method::MethodEnum method_;
    std::string path_;

};

template<http::Method::MethodEnum M, typename F>
class RestAPI : public RestBaseAPI {
public:
    RestAPI(const std::string& path, F&& f) : 
        RestBaseAPI(M, path),
        f_(std::forward<F>(f)) {

    }

    void invoke() override {
        f_();
    }

    // void invoke(Serializable arg) override {
    //     f_(arg);
    // }

    void invoke(std::initializer_list<Serializable> args) override {
        // size_t nargs = sizeof...(typename Fn_type<F>::args_tuple);
        // std::cout << "# of args: " << nargs << std::endl;
        // f_();
    }

    template<typename... Args>
    auto invoke(Args&&... args) {
        return f_(args...);
    }

    std::string getPath() const { return path_; }

private:
    F f_;
};

template<http::Method::MethodEnum M, typename F>
RestAPI(const std::string& path, F&& f) -> RestAPI<M, typename Fn_type<decltype(&F::operator())>::type>;

template<http::Method::MethodEnum M, typename F>
RestAPI(const std::string& path, F* f) -> RestAPI<M, F>;

} // namespace lattice

#endif