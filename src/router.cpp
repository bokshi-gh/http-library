#include "router.hpp"
#include "router_helpers.hpp"
#include <iostream>

void Router::add(const string& method, const string& path, RouteHandler route_handler) {
    validate_path(path);
    string normalized_path = normalize_path(path);
    routing_table[{method, normalized_path}] = route_handler;
}

void Router::handle_client(int client_fd) {
    char buffer[4096];
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
    response.headers["Server"] = "ProductName/Version (Optional comment)";
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
