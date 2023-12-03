Elastic Jeans
===
HTTP web application written in C++

## Build
### Prerequisites
The following tools have to be installed in your linux environment
- C++20 STL
- cmake (version 3.22 or later)
- make (version 4.3 or later)
#### Libraries
- OpenSSL ([GitHub](https://github.com/openssl/openssl))

### Command
```
cmake CMakeLists.txt && make
```

## Usage
```
#include <elasticJeans/jeans.hpp>
namespace ej = elasticJeans;
int main() {
    std::string ip = "127.0.0.1";
    int port = 8080;
    
    ej::app()
        .http(ip, port)
        .start();

    return 0;
}
```
