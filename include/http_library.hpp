#pragma once

#include "http_codec.hpp"
#include "dotenv_cpp.hpp"
#include "router.hpp"

#include <string>
#include <unordered_map>
#include <functional>
#include <thread>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

class HTTPServer {
private:
    int server_fd;
    uint16_t port;
    Router router;

public:
    HTTPServer();
    ~HTTPServer();

    void get(const string& path, RouteHandler route_handler);

    void listen(uint16_t port, function<void()> callback = nullptr);

private:
    string getHTTPDate();
};
