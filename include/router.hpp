#pragma once

#include "http_codec.hpp"
#include "router_helpers.hpp"

#include <string>
#include <unordered_map>
#include <functional>
#include <unistd.h>
#include <sys/socket.h>

using namespace std;
using RouteHandler = function<void(HTTPRequest& request, HTTPResponse& response)>;

struct Route {
    string method;
    string path;

    bool operator==(const Route& other) const {
        return method == other.method && path == other.path;
    }
};

struct RouteHash {
    size_t operator()(const Route& r) const {
        return hash<string>{}(r.method) ^ (hash<string>{}(r.path) * 31);
    }
};

class Router {
private:
    unordered_map<Route, RouteHandler, RouteHash> routing_table;

public:
    void add_entry_to_routing_table(const string& method, const string& path, RouteHandler route_handler);

    void handle_client(int client_fd);
};
