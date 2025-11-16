#pragma once

#include <unordered_map>
#include <functional>
#include <cstdint>
#include <string>

class HTTPRequest;
class HTTPResponse;

class Server {
private:
    int server_fd;
    uint16_t port;
    std::unordered_map<std::string, std::function<void(HTTPRequest&, HTTPResponse&)>> route_table;

    void handle_client(int client_fd);

public:
    Server();
    ~Server();

    void get(std::string route, std::function<void(HTTPRequest&, HTTPResponse&)> route_handler);
    void post(std::string route, std::function<void(HTTPRequest&, HTTPResponse&)> route_handler);

    void listen(uint16_t port, std::function<void()> callback = nullptr);
};
