#include <unordered_map>
#include <string>

namespace elasticJeans {
namespace http {

class Headers {
public:

    std::string get(const std::string& headerName);

    void set(std::string headerName, std::string value);

    std::string str();

private:
    std::unordered_map<std::string, std::string> m_headers;

};

} // http
} // elasticJeans