#include "http_server.hpp"

HTTPServer::HTTPServer() : server_fd(-1) {}

HTTPServer::~HTTPServer() {
    if (server_fd >= 0) close(server_fd);
}

void HTTPServer::http_get(const string& path, RouteHandler route_handler) {
    router.register_handler("GET", path, route_handler);
}

void HTTPServer::http_post(const string& path, RouteHandler route_handler) {
    router.register_handler("POST", path, route_handler);
}

void HTTPServer::http_put(const string& path, RouteHandler route_handler) {
    router.register_handler("PUT", path, route_handler);
}

void HTTPServer::http_delete(const string& path, RouteHandler route_handler) {
    router.register_handler("DELETE", path, route_handler);
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
            handle_client(client_fd);
        }).detach();
    }
}

void HTTPServer::handle_client(int client_fd) {
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

    bool found = router.try_dispatch(request, response);
    if (!found) {
        response.status_code = 404;
        response.reason_phrase = "Not Found";
        response.body = "404 Not Found";
    }

    // Why AFTER try_dispatch? Because we cannot know the size of the 
    // body until the handler (or the 404 logic) has finished filling it.
    // Why we ALWAYS overwrite (no IF check): Protocol Safety. If the 
    // Content-Length doesn't match the actual body size exactly, 
    // the browser will hang or the connection will break.
    response.headers["Content-Length"] = to_string(response.body.size());

    // Why AFTER try_dispatch? Because we want the 'Date' to reflect the 
    // exact moment the handler finished processing and the response is ready.
    // Why the IF check? To respect the developer; if they manually set a 
    // specific 'Date' in the handler, we don't want to overwrite their choice.
    if (response.headers.find("Date") == response.headers.end()) {
        response.headers["Date"] = get_current_date();
    }

    string raw_response = encode_http_response(response);
    send(client_fd, raw_response.c_str(), raw_response.size(), 0);

    close(client_fd);
}
