#pragma once

#include "http_codec.hpp"
#include "router_helpers.hpp"

#include <string>
#include <functional>

using namespace std;
using RouteHandler = function<void(HTTPRequest, HTTPResponse&)>;

struct Route {
  string method;
  string path;

  bool operator==(const Route& other) const {
    return method == other.method && path == other.path;
  }
}

struct RouteHash {
    std::size_t operator()(const Route& r) const {
        std::size_t h1 = std::hash<std::string>{}(r.method);
        std::size_t h2 = std::hash<std::string>{}(r.path);
        return h1 ^ (h2 * 31); // combine hashes
    }
};

class Router {
private:
  unordered_map<Route, RouteHandler, RouteHash> routing_table;

public:
  void add(const string method, string path, RouteHandler, route_handler);
}
