#pragma once

#include "http_codec.hpp"
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

    void http_get(const std::string& path, RouteHandler handler);
    void http_post(const std::string& path, RouteHandler handler);
    void http_put(const std::string& path, RouteHandler handler);
    void http_delete(const std::string& path, RouteHandler handler);

    void listen(uint16_t port, function<void()> callback = nullptr);
   
private:
    void handle_incoming_client(int client_fd);
};
