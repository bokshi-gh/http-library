#include "../include/http_library.hpp"
#include <iostream>

int main() {
    Server srv;

    srv.get("/monk", [&](HTTPRequest& request, HTTPResponse& response) {
        response.headers["Content-Type"] = "text/html";
        response.body = "<p>a monk in a cloud</p>";
    });

    srv.listen(8080, [&]() {
        std::cout << "server is listening on port 8080\n";
    });

    return 0;
}
