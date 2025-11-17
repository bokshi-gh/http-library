#include "http_library.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <iostream>

Server::Server() : server_fd(-1), port(0) {}

Server::~Server() {
    if (server_fd >= 0) close(server_fd);
}

void Server::get(string route, function<void(HTTPRequest&, HTTPResponse&)> route_handler) {
    route_table[route] = route_handler;
}

void Server::listen(uint16_t port, function<void()> callback) {
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

    if (callback) {
        callback();
        cout.flush();
    }

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        thread(&Server::handle_client, this, client_fd).detach();
    }
}

void Server::handle_client(int client_fd) {
    char buffer[4096]; // WARNING: buffer can't handle payload greater than 4096
    int n = read(client_fd, buffer, sizeof(buffer));
    if (n <= 0) {
        close(client_fd);
        return;
    }
    buffer[n] = '\0'; // since read function doesn't null terminate the array we have to do it manually

    HTTPRequest request = decode_http_request(buffer);
    HTTPResponse response;

    // assign default value
    response.version = "HTTP/1.1";
    response.status_code = 200;
    response.reason_phrase = "OK";

    if (route_table.find(request.path) != route_table.end()) {
        route_table[request.path](request, response);
    } else {
        response.status_code = 404;
        response.reason_phrase = "Not Found";
    }

    string raw_response = encode_http_response(response);
    send(client_fd, raw_response.c_str(), raw_response.size(), 0);

    close(client_fd);
}
