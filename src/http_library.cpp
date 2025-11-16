#include "http_library.hpp"
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

Server::Server() {
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024];

    this->server_fd = (AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        return 1;
    }
}

Server::~Server() {
    close(server_fd);
    close(client_fd);
}

Server::get(std::string route, std::function<void(int)> route_handler) {
    route_table[route] = route_handler;
}

Server::handle_client(int client_fd) {
    // check path
} 

Server::listen(uint16_t port, std::function<void(int)> callback) {
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;   // 0.0.0.0
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        return 1;
    }

    callback();

    while (true) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        // client_addr.sin_addr

        std::thread t(handle_client, int client_fd);

        close(client_fd);
    }
}