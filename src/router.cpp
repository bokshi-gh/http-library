#include "router.hpp"

void Router::add(const string method, string path, RouteHandler route_handler) {
  routing_table[{method, path}] = route_handler;
}
