#include "http_server.hpp"

HTTPServer::HTTPServer() : server_fd(-1) {}

HTTPServer::~HTTPServer() {
    if (server_fd >= 0) close(server_fd);
}

void HTTPServer::get(const string& path, RouteHandler route_handler) {
    router.register_handler("GET", path, route_handler);
}

void HTTPServer::post(const string& path, RouteHandler route_handler) {
    router.register_handler("POST", path, route_handler);
}

void HTTPServer::put(const string& path, RouteHandler route_handler) {
    router.register_handler("PUT", path, route_handler);
}

void HTTPServer::del(const string& path, RouteHandler route_handler) {
    router.register_handler("DELETE", path, route_handler);
}

void HTTPServer::listen(uint16_t port, function<void()> callback) {
    this->port = port;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket failed"); return; }

    // Allow socket reuse immediately after close
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

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
    char buffer[4096];
    const int keep_alive_timeout = 5; // seconds

    while (true) {
        string raw_request;
        ssize_t n = 0;

        // Set a timeout for recv (so it won't block forever)
        timeval tv{};
        tv.tv_sec = keep_alive_timeout;
        tv.tv_usec = 0;
        setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

        // Read request headers
        while (true) {
            n = recv(client_fd, buffer, sizeof(buffer), 0);
            if (n < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // Timeout: close connection for keep-alive
                    close(client_fd);
                    return;
                } else {
                    perror("recv failed");
                    close(client_fd);
                    return;
                }
            } else if (n == 0) {
                // Client closed connection
                close(client_fd);
                return;
            }
            raw_request.append(buffer, n);
        }

        // Decode HTTP request
        HTTPRequest request;
        try {
            request = decode_http_request(raw_request.c_str());
        } catch (...) {
            // Bad request
            string bad_response = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: 0\r\n\r\n";
            send(client_fd, bad_response.c_str(), bad_response.size(), 0);
            close(client_fd);
            return;
        }

        // Prepare response
        HTTPResponse response;
        response.version = "HTTP/1.1";
        response.status_code = 200;
        response.reason_phrase = "OK";
        response.headers["Content-Type"] = "text/plain";
        response.headers["Server"] = "ProductName/Version";

        bool found = router.try_dispatch(request, response);
        if (!found) {
            response.status_code = 404;
            response.reason_phrase = "Not Found";
            response.body = "404 Not Found";
        }

        // Date header
        if (response.headers.find("Date") == response.headers.end()) {
            response.headers["Date"] = get_current_date();
        }

        // Determine if connection should be kept alive
        bool keep_alive = false;
        auto it = request.headers.find("Connection");
        if (it != request.headers.end() &&
            (it->second == "keep-alive" || it->second == "Keep-Alive")) {
            keep_alive = true;
            response.headers["Connection"] = "keep-alive";
            response.headers["Keep-Alive"] = "timeout=" + to_string(keep_alive_timeout);
        } else {
            response.headers["Connection"] = "close";
        }

        // Encode and send response
        string raw_response = encode_http_response(response);
        ssize_t total_sent = 0;
        ssize_t to_send = raw_response.size();
        while (total_sent < to_send) {
            ssize_t sent = send(client_fd, raw_response.c_str() + total_sent, to_send - total_sent, 0);
            if (sent <= 0) break;
            total_sent += sent;
        }

        // If not keep-alive, close the socket
        if (!keep_alive) {
            close(client_fd);
            return;
        }

        // If keep-alive, continue loop for next request
    }
}
