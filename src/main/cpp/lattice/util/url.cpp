#include "url.hpp"
#include <sstream>

namespace lattice {

std::string URL::str() {
    std::ostringstream osstream;
    osstream << scheme_ << authority_.str()
        << path_ << query_ << fragment_;

    return osstream.str();
}

URL::Authority::Authority(std::string target) {
    auto atPos = target.find("@");
    auto colonPos  = target.find(":");
    if (atPos != std::string::npos)
        userInfo = target.substr(0, atPos);
    else atPos = 0;
    if (colonPos != std::string::npos) {
        try {
            port = std::stoi(target.substr(colonPos));
        } catch (std::exception& err) {
            port = -1;
        }
    } else {
        port = -1;
        colonPos = target.size();
    }
    host = target.substr(atPos, colonPos - atPos);
}

std::string URL::Authority::str() {
    std::string authStr;
    if (userInfo.size() > 0)
        authStr += userInfo + "@";
    authStr += host;
    if (port >= 0)
        authStr += ":" + std::to_string(port);

    return authStr;
}

} // namespace lattice