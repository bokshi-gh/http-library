#pragma once

#include "http_codec.hpp"

#include <string>
#include <unordered_map>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

class HTTPClient {
public:
    static HTTPResponse http_get(const string& url, const unordered_map<string, string>& headers); 
    static HTTPResponse http_post(const string& url, const unordered_map<string, string>& headers, const string& body); 
    static HTTPResponse http_put(const string& url, const unordered_map<string, string>& headers, const string& body); 
    static HTTPResponse http_delete(const string& url, const unordered_map<string, string>& headers, const string& body); 
}
