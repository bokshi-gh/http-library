#pragma once

#include "http_codec.hpp"
#include "dotenv.hpp"
#include <cstdint>
#include <unordered_map>
#include <string>
#include <functional>

using namespace std;

class Server {
private:
    int server_fd;
    uint16_t port;
    unordered_map<string, function<void(HTTPRequest&, HTTPResponse&)>> pattern_table;

    bool match_pattern(const string pattern, const string path, HTTPRequest &request);
    string getHTTPDate();
    void handle_client(int client_fd);

public:
    Server();
    ~Server();

    void get(string pattern, function<void(HTTPRequest&, HTTPResponse&)> pattern_handler);
    void listen(uint16_t port, function<void()> callback = nullptr);
};

class Client {
private:
	string hostname;
	uint16_t port;

public:
	Client(string hostname);
	Client(string hostname, uint16_t port);

	HTTPResponse get(const string path, const unordered_map<string, string> headers = {});
};
