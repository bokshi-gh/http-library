#pragma once

#include "http_codec.hpp"

#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

struct URL {
    std::string scheme;
    std::string host;   // domain or IP (IPv4 / IPv6)
    std::string port;
    std::string path;
    std::string query;
};

using namespace std;

class HTTPClient {
public:
    static HTTPResponse http_get(const string& url, const unordered_map<string, string>& headers); 
    static HTTPResponse http_post(const string& url, const unordered_map<string, string>& headers, const string& body); 
    static HTTPResponse http_put(const string& url, const unordered_map<string, string>& headers, const string& body); 
    static HTTPResponse http_delete(const string& url, const unordered_map<string, string>& headers, const string& body);

private:
    static HTTPResponse execute_request(const HTTPRequest& req, const std::string& host, const std::string& port);
};
