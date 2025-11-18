#include "http_library.hpp"
#include <cstdint>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <cstring>
#include <netdb.h>
#include <ctime>

string Server::getHTTPDate() {
    time_t now = std::time(nullptr);
    tm gm_time{};
    gmtime_r(&now, &gm_time);

    char buf[30];
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &gm_time);
    return std::string(buf);
}

Server::Server() : server_fd(-1), port(0) {}

Server::~Server() {
    if (server_fd >= 0) close(server_fd);
}

void Server::get(string endpoint, function<void(HTTPRequest &, HTTPResponse &)> endpoint_handler) {
    endpoint_table[endpoint] = endpoint_handler;
}

void Server::listen(uint16_t port, function<void()> callback) {
    this->port = port;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket failed"); return; }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) { perror("bind failed"); return; }

    if (::listen(server_fd, 5) < 0) { perror("listen failed"); return; }

    if (callback) { callback(); cout.flush(); }

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) { perror("accept failed"); continue; }

        thread(&Server::handle_client, this, client_fd).detach();
    }
}

void Server::handle_client(int client_fd) {
    char buffer[4096]; // WARNING: buffer can't handle payload greater than 4096
    int n = read(client_fd, buffer, sizeof(buffer));
    if (n <= 0) { close(client_fd); return; }
    buffer[n] = '\0';

    HTTPRequest request = decode_http_request(buffer);
    HTTPResponse response;

    response.version = "HTTP/1.1";
    response.status_code = 200;
    response.reason_phrase = "OK";
    response.headers["Content-Type"] = "text/plain";
    response.headers["Connection"] = "keep-alive";
    response.headers["Server"] = "HTTP-Library/1.0.0 (C++ server)";
    response.headers["Date"] = getHTTPDate();
    response.headers["Cache-Control"] = "no-cache";
    
    if (endpoint_table.find(request.path) != endpoint_table.end()) {
        endpoint_table[request.path](request, response);
    } else {
        response.status_code = 404;
        response.reason_phrase = "Not Found";
    }

    if(response.headers.find("Content-Type") == response.headers.end()) response.headers["Content-Length"] = to_string(response.body.length());

    string raw_response = encode_http_response(response);
    send(client_fd, raw_response.c_str(), raw_response.size(), 0);

    close(client_fd);
}

Client::Client(string hostname) : hostname(hostname), port(80) {}

Client::Client(string hostname, uint16_t port) : hostname(hostname), port(port) {}

HTTPResponse Client::get(const string endpoint, const unordered_map<string, string> headers) {
    string raw_request = "GET " + endpoint + " HTTP/1.1\r\n";
    if(headers.find("Host") == headers.end()) raw_request = raw_request + "Host: " + hostname + "\r\n";
    if(headers.find("Connection") == headers.end()) raw_request = raw_request + "Connection: keep-alive" + "\r\n";
    if(headers.find("User-Agent") == headers.end()) raw_request = raw_request + "User-Agent: HTTP-Library/1.0.0 (C++ client)" + "\r\n";
    if(headers.find("Accept") == headers.end()) raw_request = raw_request + "Accept: */*" + "\r\n";

    for (auto header : headers) {
        raw_request += header.first + ": " + header.second + "\r\n";
    }
    
    raw_request += "\r\n";

    HTTPResponse empty_response{};

    struct hostent *host = gethostbyname(hostname.c_str());
    if (!host) { fprintf(stderr, "Unknown host: %s\n", hostname.c_str()); return empty_response; }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("Socket creation failed"); return empty_response; }

    struct sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr = *((struct in_addr *)host->h_addr);
    memset(&(server.sin_zero), 0, 8);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) { perror("Connect failed"); close(sock); return empty_response; }

    send(sock, raw_request.c_str(), raw_request.length(), 0);

    char buffer[4096];
    string data;
    while (true) {
        ssize_t bytes = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes > 0) {
            data.append(buffer, bytes);
        } else if (bytes == 0) {
            close(sock); 
            exit(EXIT_FAILURE);
        } else {
            if (errno == EINTR) continue;   // interrupted? retry
            perror("recv");
            close(sock); 
            exit(EXIT_FAILURE);
        }
    }

    close(sock);

    return decode_http_response(data.c_str());
}
