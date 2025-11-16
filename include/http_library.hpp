#pragma once

#include <unordered_map>
#include <functional>

class Server {
private:
    int server_fd;
    int client_fd;
    uint16_t port;
    unordered_map route_table

public:
    Server();
    ~Server();

    get(std::string route, std::function<void(int)> route_handler);
    post(std::string route, std::function<void(int)> route_handler);

    handle_client(int client_fd);
    listen(uint16_t port, std::function<void(int)> callback);
}