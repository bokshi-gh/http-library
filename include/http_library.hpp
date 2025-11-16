#pragma once

#include <unordered_map>
#include <functional>
#include <cstdint>

class Server {
private:
    int server_fd;
    int client_fd;
    uint16_t port;
    unordered_map<string, std::function<void(HTTPRequest, HTTPResponse)>> route_table;

public:
    Server();
    ~Server();

    get(std::string route, std::function<void(HTTPRequest, HTTPResponse)> route_handler);
    post(std::string route, std::function<void(HTTPRequest, HTTPResponse)> route_handler);

    listen(uint16_t port, std::function<void(int)> callback);
    handle_client(int client_fd);
};