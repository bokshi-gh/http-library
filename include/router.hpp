#pragma once

#include "http_codec.hpp"
#include "router_helpers.hpp"

#include <string>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <iostream>

using namespace std;

using RouteHandler = function<void(HTTPRequest, HTTPResponse&)>;

struct Route {
    string method;
    string path;

    bool operator==(const Route& other) const {
        return method == other.method && path == other.path;
    }
};

struct RouteHash {
    size_t operator()(const Route& r) const {
        size_t h1 = hash<string>{}(r.method);
        size_t h2 = hash<string>{}(r.path);
        return h1 ^ (h2 * 31); // combine hashes
    }
};

class Router {
private:
    unordered_map<Route, RouteHandler, RouteHash> routing_table;

public:
    void add(const string& method, const string& path, RouteHandler route_handler);

    void handle_client(int client_fd);

    // Optional: simple pattern matching (like /user/:id)
    bool match_pattern(const string& pattern, const string& path, HTTPRequest& request);
};
