#pragma once

#include "http_codec.hpp"
#include "dotenv.hpp"
#include <cstdint>
#include <unordered_map>
#include <string>
#include <functional>

class Server {
private:
    int server_fd;
    uint16_t port;
    unordered_map<string, function<void(HTTPRequest&, HTTPResponse&)>> endpoint_table;

    void handle_client(int client_fd);

public:
    Server();
    ~Server();

    void get(string route, function<void(HTTPRequest&, HTTPResponse&)> endpoint_handler);
    void listen(uint16_t port, function<void()> callback = nullptr);
};

class Client {
private:
	string hostname;
	uint16_t port;

public:
	Client(string hostname);
	Client(string hostname, uint16_t port);

	HTTPResponse get(const std::string endpoint, const std::unordered_map<std::string, std::string> headers = {});
};
