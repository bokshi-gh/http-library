#include "http_library.hpp"

#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
    // Load .env file
    Dotenv env;

    // Get server port from .env or default to 8080
    uint16_t port = static_cast<uint16_t>(
        stoi(env.get("SERVER_PORT", "8080"))
    );

    HTTPServer server;

    // GET route
    server.http_get("/hello", [](HTTPRequest& req, HTTPResponse& res){
        res.body = "Hello, world!";
        cout << "GET /hello called" << endl;
    });

    // POST route
    server.http_post("/submit", [](HTTPRequest& req, HTTPResponse& res){
        res.body = "Data submitted successfully!";
        cout << "POST /submit called" << endl;
    });

    // PUT route
    server.http_put("/update", [](HTTPRequest& req, HTTPResponse& res){
        res.body = "Resource updated!";
        cout << "PUT /update called" << endl;
    });

    // DELETE route
    server.http_delete("/remove", [](HTTPRequest& req, HTTPResponse& res){
        res.body = "Resource deleted!";
        cout << "DELETE /remove called" << endl;
    });

    // Start server
    cout << "Server starting on port " << port << "..." << endl;
    server.listen(port, [](){
        cout << "Server started successfully!" << endl;
    });

    return 0;
}
