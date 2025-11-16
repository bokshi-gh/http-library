#include "../include/http_codec.hpp"
#include "../include/http_library.hpp"
#include <iostream>
#include <string>

int main() {
    Server srv;

    srv.get("/monk", [&](HTTPRequest& request, HTTPResponse& response) {
	response.version = "HTTP/1/1";
	response.status_code = 200;
	response.reason_phrase = "Ok";
        response.headers["Content-Type"] = "text/html";
        response.body = "<p>a monk in a cloud</p>";
    });

    srv.listen(8080, [&]() {
        std::cout << "server is listening on port 8080\n";
	std::cout << "yo";
    });

    return 0;
}
