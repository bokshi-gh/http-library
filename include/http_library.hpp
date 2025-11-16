#pragma once

#include <functional>

class Server {
private:
    int server_fd;
    int client_fd;
    uint16_t port;

public:
    Server();
    ~Server();

    listen(uint16_t port, std::function<void(int)> callback);
}