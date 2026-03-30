#include "router.hpp"
#include "router_helpers.hpp"
#include <iostream>

void Router::add_entry_to_routing_table(const string& method, const string& path, RouteHandler route_handler) {
    validate_path(path);
    string normalized_path = normalize_path(path);
    routing_table[{method, normalized_path}] = route_handler;
}


