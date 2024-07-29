#ifndef _LATTICE_URL_H
#define _LATTICE_URL_H

#include "uri.hpp"
#include <string>
#include <exception>

namespace lattice {

class URL : public URI {
public:
    struct Authority {
        std::string userInfo;
        std::string host;
        int port;
        Authority(std::string target);
        std::string str();
    };

    URL(std::string scheme, 
            std::string authority,
            std::string path, 
            std::string query, 
            std::string fragment) :
        URL(scheme, Authority(authority), path, query, fragment) {}

    URL(std::string scheme, 
            const struct Authority& authority,
            std::string path, 
            std::string query, 
            std::string fragment) :
        URI(), scheme_(scheme), authority_(authority), path_(path), query_(query), fragment_(fragment) {}

    std::string str() override;

private:
    std::string scheme_;
    struct Authority authority_;
    std::string path_;
    std::string query_;
    std::string fragment_;

};

} // namespace lattice
#endif // _LATTICE_URL_H