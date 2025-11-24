#include <http_library.hpp>
#include <http_codec.hpp>
#include <dotenv.hpp>
#include <iostream>
#include <string>
#include <cstdlib>

int main (int argc, char *argv[]) {
        dotenv::init();
        std::string PORT = dotenv::getenv("PORT");

        Server server;
        server.get("/monk", [](HTTPRequest &req, HTTPResponse &res) {
                res.headers["Content-Type"] = "text/html";
                res.body = "<p>a monk in a cloud!</p>";
        });
        server.listen(atoi(PORT.c_str()), [&]() {
                std::cout << "HTTP server is listening on: 0.0.0.0:" << atoi(PORT.c_str());
        });
	
        return 0;
}
