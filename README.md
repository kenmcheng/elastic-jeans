Elastic Jeans
===
HTTP web application written in C++

## Build
### Supported OS
- Linux
### Prerequisites
The following tools have to be installed in your linux environment
- gcc/g++ 13
- C++20 STL
- cmake (version 3.22 or later)
- make (version 4.3 or later)
#### Libraries
- OpenSSL ([GitHub](https://github.com/openssl/openssl))
#### Libraries for test
- GoogleTest ([Website](https://google.github.io/googletest/), [GitHub](https://github.com/google/googletest))

### Command
```
cmake -S . -B build
```
```
cmake --build build
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
