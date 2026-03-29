#include "http_library.hpp"

void remove_trailing_forward_slash(string& str) {
  if (str.size() > 1 && str.back() == '/') str.pop_back();
}

void Server::get(string path, function<void(HTTPRequest, HTTPResponse&)> route_handler) {
  remove_trailing_forward_slash(path);

  routing_table[path] = route_handler;
}
