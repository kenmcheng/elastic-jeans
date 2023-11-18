#ifndef _ELASTICJEANS_HTTP_HEADER_H
#define _ELASTICJEANS_HTTP_HEADER_H
#include <unordered_map>
#include <string>
#include <vector>

namespace elasticJeans {
namespace http {

const std::string CRLF = "\r\n";

class Header {
public:
    using vec_str = std::vector<std::string>;

    std::string& operator[](size_t idx) { return values_[idx]; }

    Header& operator=(std::string value) {
        this->values_.push_back(std::move(value));
        return *this;
    }

    size_t size() { return values_.size(); }

    typename vec_str::iterator begin() { return values_.begin(); }
    typename vec_str::const_iterator begin() const { return values_.begin(); }
    typename vec_str::iterator end() { return values_.end(); }
    typename vec_str::const_iterator end() const { return values_.end(); }

    void clear() { values_.clear(); }

private:
    std::string name_;
    vec_str values_;
};

class HeaderList {
public:

    Header& operator[](const std::string& headerName);

    Header& get(const std::string& headerName);

    void set(std::string headerName, std::string value);

    std::string str() const;

    void clear();

    bool exist(const std::string& key) { return headers_.count(key); } 

private:
    std::unordered_map<std::string, Header> headers_;

};

} // http
} // elasticJeans

#endif