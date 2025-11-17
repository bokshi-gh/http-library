#pragma once

#include <string>
#include <unordered_map>

using namespace std;

class HTTPRequest {
public:
    string raw;
    string method;
    string path;
    string version;
    unordered_map<string, string> headers;
    string body;
};

class HTTPResponse {
public:
    string raw;
    string version;
    unsigned short int status_code;
    string reason_phrase;
    unordered_map<string, string> headers;
    string body;
};

HTTPRequest decode_http_request(const char *raw_request);
HTTPResponse decode_http_response(const char *raw_response);

string encode_http_request(const HTTPRequest &request);
string encode_http_response(const HTTPResponse &response);
