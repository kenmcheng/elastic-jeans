#include "http/http_linux.hpp"
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace elasticJeans;

int main() {
    http::HttpServer server("127.0.0.1", 8080);
    server.start();
    return 0;
}