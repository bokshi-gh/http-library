#pragma once

#include "http_codec.hpp"
#include <unordered_map>
#include <functional>
#include <cstdint>
#include <string>


class Server {
private:
    int server_fd;
    uint16_t port;
    unordered_map<string, function<void(HTTPRequest&, HTTPResponse&)>> route_table;

    void handle_client(int client_fd);

public:
    Server();
    ~Server();

    void get(string route, function<void(HTTPRequest&, HTTPResponse&)> route_handler);
    void post(string route, function<void(HTTPRequest&, HTTPResponse&)> route_handler);

    void listen(uint16_t port, function<void()> callback = nullptr); // callback parameter becomes nullptr if second argument is not passed when calling the listen function
};
