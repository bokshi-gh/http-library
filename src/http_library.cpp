#include "http_library.hpp"
#include "http_codec.hpp"
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

Server::Server() : server_fd(-1), port(0) {}

Server::~Server() {
    if (server_fd >= 0) close(server_fd);
}

void Server::get(std::string route, std::function<void(HTTPRequest, HTTPResponse)> route_handler) {
    route_table[route] = route_handler;
}

void Server::handle_client(int client_fd) {
    char buffer[4096];
    int n = read(client_fd, buffer, sizeof(buffer));
    if (n <= 0) {
        close(client_fd);
        return;
    }

    std::string raw_request(buffer, n); // this is redundant
    HTTPRequest request = decode_http_request(raw_request.c_str());
    HTTPResponse response;

    if (route_table.find(request.path) != route_table.end()) {
        route_table[request.path](request, response);
    } else {
        response.status_code = 404;
        response.body = "Not Found";
    }

    std::string raw_response = encode_http_response(response);
    send(client_fd, raw_response.c_str(), raw_response.size(), 0);

    close(client_fd);
}

void Server::listen(uint16_t port, std::function<void()> callback) {
    this->port = port;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        return;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        return;
    }

    if (::listen(server_fd, 5) < 0) {
        perror("listen failed");
        return;
    }

    if (callback) callback();

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        std::thread(&Server::handle_client, this, client_fd).detach();
    }
}