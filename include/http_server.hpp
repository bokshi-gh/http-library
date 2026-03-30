#pragma once

#include "server_helpers.hpp"

#include "router.hpp"
#include "codec.hpp"

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

    void get(const std::string& path, RouteHandler route_handler);
    void post(const std::string& path, RouteHandler route_handler);
    void put(const std::string& path, RouteHandler route_handler);
    void del(const std::string& path, RouteHandler route_handler);

    void listen(uint16_t port, function<void()> callback = nullptr);
   
private:
    void handle_client(int client_fd);
};
