#include "router.hpp"
#include "router_helpers.hpp"
#include <iostream>

void Router::add_entry_to_routing_table(const string& method, const string& path, RouteHandler route_handler) {
    validate_path(path);
    string normalized_path = normalize_path(path);
    routing_table[{method, normalized_path}] = route_handler;
}

bool Router::find_and_invoke_handler(HTTPRequest& request, HTTPResponse& response) {
    bool found = false;
    for (auto& pair : routing_table) {
        if (match_route(pair.first.path, request.path, request) &&
            pair.first.method == request.method) {
            pair.second(request, response);
            found = true;
            break;
        }
    }
    return found;
}
