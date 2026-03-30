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
            handle_incoming_client(client_fd);
        }).detach();
    }
}

void HTTPServer::handle_incoming_client(int client_fd) {
    char buffer[4096];  // WARNING: buffer can't handle payload greater than 4096
    int n = read(client_fd, buffer, sizeof(buffer) - 1);
    if (n <= 0) { close(client_fd); return; }
    buffer[n] = '\0';

    HTTPRequest request = decode_http_request(buffer);
    HTTPResponse response;

    response.version = "HTTP/1.1";
    response.status_code = 200;
    response.reason_phrase = "OK";
    response.headers["Content-Type"] = "text/plain";
    response.headers["Connection"] = "keep-alive";
    response.headers["Server"] = "ProductName/Version (Optional comment)";  // will fix this later
    response.headers["Date"] = get_current_date();

    bool found = false;
    for (auto& pair : routing_table) {
        if (match_route(pair.first.path, request.path, request) &&
            pair.first.method == request.method) {
            pair.second(request, response);
            found = true;
            break;
        }
    }

    if (!found) {
        response.status_code = 404;
        response.reason_phrase = "Not Found";
        response.body = "404 Not Found";
    }

    if (response.headers.find("Content-Length") == response.headers.end()) {
        response.headers["Content-Length"] = to_string(response.body.size());
    }

    string raw_response = encode_http_response(response);
    send(client_fd, raw_response.c_str(), raw_response.size(), 0);

    close(client_fd);
}
