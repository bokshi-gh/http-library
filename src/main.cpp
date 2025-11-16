#include "http_codec.hpp"
#include "http_library.hpp"
#include <iostream>
#include <string>

int main() {
    Server srv;

    srv.get("/", [&](HTTPRequest& request, HTTPResponse& response) {
        response.headers["Content-Type"] = "text/html";
        response.body = "<p>a monk in a cloud</p>";
    });

    srv.listen(8080, [&]() {
        std::cout << "Server is listening on port 8080\n";
    });

    return 0;
}