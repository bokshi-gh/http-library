#include "http_codec.hpp"
#include <cstring>

HTTPRequest decode_http_request(const char* raw_request) {
    HTTPRequest request;
    request.raw = std::string(raw_request);

    size_t len = std::strlen(raw_request) + 1;
    char* buffer = new char[len];
    std::strcpy(buffer, raw_request);

    char* line = strtok(buffer, "\r\n");
    if (line) {
        char* token = strtok(line, " ");
        if (token) request.method = token;

        token = strtok(nullptr, " ");
        if (token) request.path = token;

        token = strtok(nullptr, " ");
        if (token) request.version = token;
    }

    while ((line = strtok(nullptr, "\r\n")) != nullptr) {
        if (line[0] == '\0') {
            // Empty line → body starts
            break;
        }
        std::string header_line(line);
        size_t colon_pos = header_line.find(':');
        if (colon_pos != std::string::npos) {
            std::string key = header_line.substr(0, colon_pos);
            std::string value = header_line.substr(colon_pos + 1);
            size_t first = value.find_first_not_of(" ");
            if (first != std::string::npos)
                value = value.substr(first);
            request.headers[key] = value;
        }
    }

    // everything after empty line
    char* body = strtok(nullptr, ""); // remaining string
    if (body) {
        request.body = body;
    }

    delete[] buffer;
    return request;
}

HTTPResponse decode_http_response(const char* raw_response) {
    HTTPResponse response;
    response.raw = std::string(raw_response);

    size_t len = std::strlen(raw_response) + 1;
    char* buffer = new char[len];
    std::strcpy(buffer, raw_response);

    char* line = strtok(buffer, "\r\n");
    if (line) {
        char* token = strtok(line, " ");
        if (token) response.version = token;

        token = strtok(nullptr, " ");
        if (token) response.status_code = atoi(token);

        token = strtok(nullptr, " ");
        if (token) response.reason_phrase = token;
    }

    while ((line = strtok(nullptr, "\r\n")) != nullptr) {
        if (line[0] == '\0') {
            // Empty line → body starts
            break;
        }
        std::string header_line(line);
        size_t colon_pos = header_line.find(':');
        if (colon_pos != std::string::npos) {
            std::string key = header_line.substr(0, colon_pos);
            std::string value = header_line.substr(colon_pos + 1);
            size_t first = value.find_first_not_of(" ");
            if (first != std::string::npos)
                value = value.substr(first);
            response.headers[key] = value;
        }
    }

    // everything after empty line
    char* body = strtok(nullptr, ""); // remaining string
    if (body) {
        response.body = body;
    }

    delete[] buffer;
    return response;
}

string encode_http_request(const HTTPRequest &request) {
    if (request.method.empty() || request.path.empty() || request.version.empty()) {
        // cannot form valid request
        return "";   // or throw exception / return error code
    }

    string raw_request = request.method + " " + request.path + " " + request.version + "\r\n";

    // Append headers
    for (const auto &header : request.headers) {
        raw_request += header.first + ": " + header.second + "\r\n";
    }

    // End of headers
    raw_request += "\r\n";

    // Append body (can be empty)
    raw_request += request.body;

    return raw_request;
}

string encode_http_response(const HTTPResponse &response) {
    // WARNING: not handled for request.status_code
    if (response.version.empty() || response.reason_phrase.empty()) {
        // cannot form valid request
        return "";   // or throw exception / return error code
    }

    string raw_response = response.version + " " + to_string(response.status_code) + " " + response.reason_phrase + "\r\n";

    // Append headers
    for (const auto &header : response.headers) {
        raw_response += header.first + ": " + header.second + "\r\n";
    }

    // End of headers
    raw_response += "\r\n";

    // Append body (can be empty)
    raw_response += response.body;

    return raw_response;
}