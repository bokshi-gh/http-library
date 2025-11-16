#include "http_codec.hpp"
#include "http_library.hpp"

int main() {
    Server srv;
    srv.get("/", [&](HTTPRequest request, HTTPResponse response){
        
    });
    return 0;
}