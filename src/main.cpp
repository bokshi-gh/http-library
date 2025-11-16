#include "http_codec.hpp"
#include "http_library.hpp"

int main() {
    Server srv;
    srv.get("/", [&](HTTPRequest request, HTTPResponse response) {
        response.headers["Content-Type"] = "text/html";
        response.body = "<p>a monk in a cloud</p>";
    });
    return 0;
}