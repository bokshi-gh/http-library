#include "router.hpp"

#include <iostream>

void Router::register_handler(const string& method, const string& path, RouteHandler route_handler) {
    validate_path(path);
    routing_table[{method, path}] = route_handler;
}

bool Router::try_dispatch(HTTPRequest& request, HTTPResponse& response) {
    for (auto& pair : routing_table) {
        if (match_route(pair.first.path, request.path, request) &&
            pair.first.method == request.method) {
            pair.second(request, response);
            return true;
        }
    }
    return false;
}
