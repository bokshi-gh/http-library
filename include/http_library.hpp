#pragma once

#include "http_codec.hpp"
#include "dotenv_cpp.hpp"
#include "router.hpp"

#include <string>
#include <unordered_map>

using namespace std;

class Server {
  private:
    int server_fd;
    uint16_t port;

  public:
    Server();
    ~Server();

    void get(string path, RouteHandler route_handler);

    void listen(uint16_t port, function<void()> callback = nullptr);
}
