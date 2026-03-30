#include "http_library.hpp"

HTTPServer::HTTPServer() : server_fd(-1) {}

HTTPServer::~HTTPServer() {
    if (server_fd >= 0) close(server_fd);
}

void HTTPServer::http_get(const string& path, RouteHandler route_handler) {
    router.add_entry_to_routing_table("GET", path, route_handler);
}

void HTTPServer::http_post(const string& path, RouteHandler route_handler) {
    router.add_entry_to_routing_table("POST", path, route_handler);
}

void HTTPServer::http_put(const string& path, RouteHandler route_handler) {
    router.add_entry_to_routing_table("PUT", path, route_handler);
}

void HTTPServer::http_delete(const string& path, RouteHandler route_handler) {
    router.add_entry_to_routing_table("DELETE", path, route_handler);
}

void HTTPServer::listen(uint16_t port, function<void()> callback) {
    this->port = port;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket failed"); return; }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed"); return;
    }

    if (::listen(server_fd, 5) < 0) { perror("listen failed"); return; }

    if (callback) { callback(); cout.flush(); }

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) { perror("accept failed"); continue; }

        thread([this, client_fd]() {
            router.handle_client(client_fd);
        }).detach();
    }
}
