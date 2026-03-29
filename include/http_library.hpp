#pragma once

#include "http_codec.hpp"
#include "dotenv_cpp.hpp"

#include <unordered_map>

using namespace std;

struct Route {
  string method;
  string path;
}

class Server {
  private:
    int server_fd;
    uint16_t port;

    unordered_map<Route, function<void(HTTPRequest, HTTPResponse&)>> routing_table;

  public:
    Server();
    ~Server();

    void get(string path, function<void(HTTPRequest, HTTPResponse&)> route_handler);

    void listen(uint16_t port, function<void()> callback = nullptr);
}
