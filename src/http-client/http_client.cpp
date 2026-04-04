#include "http_client.hpp"
#include "http_client_helpers.hpp"

HTTPResponse execute_request(const HTTPRequest& req,
                                    const std::string& host,
                                    const std::string& port) {

    struct addrinfo hints{}, *res;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0) {
        throw std::runtime_error("DNS resolution failed");
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) {
        freeaddrinfo(res);
        throw std::runtime_error("Socket creation failed");
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) != 0) {
        close(sockfd);
        freeaddrinfo(res);
        throw std::runtime_error("Connection failed");
    }

    freeaddrinfo(res);

    // Encode request
    std::string raw_request = encode_http_request(req);

    // Send FULL request
    size_t total_sent = 0;
    while (total_sent < raw_request.size()) {
        ssize_t sent = send(sockfd,
                            raw_request.c_str() + total_sent,
                            raw_request.size() - total_sent,
                            0);

        if (sent <= 0) {
            close(sockfd);
            throw std::runtime_error("Send failed");
        }

        total_sent += sent;
    }

    // Receive FULL response
    std::string raw_response;
    char buffer[4096];

    ssize_t bytes;
    while ((bytes = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        raw_response.append(buffer, bytes);
    }

    close(sockfd);

    // Decode response
    return decode_http_response(raw_response.c_str());
}

HTTPResponse HTTPClient::http_get(const std::string& url_str,
                                  const std::unordered_map<std::string, std::string>& headers) {

    URL url = parse_url(url_str);

    if (url.scheme != "http") {
        throw std::runtime_error("Only HTTP supported");
    }

    HTTPRequest req;
    req.method = "GET";
    req.request_target = build_target(url);
    req.version = "HTTP/1.1";

    req.headers["host"] = {url.host};
    req.headers["connection"] = {"close"};

    for (auto& h : headers) {
        req.headers[h.first] = {h.second};
    }

    return execute_request(req, url.host, url.port);
}

HTTPResponse HTTPClient::http_post(const std::string& url_str,
                                   const std::unordered_map<std::string, std::string>& headers,
                                   const std::string& body) {

    URL url = parse_url(url_str);

    if (url.scheme != "http") {
        throw std::runtime_error("Only HTTP supported");
    }

    HTTPRequest req;
    req.method = "POST";
    req.request_target = build_target(url);
    req.version = "HTTP/1.1";
    req.body = body;

    req.headers["host"] = {url.host};
    req.headers["connection"] = {"close"};
    req.headers["content-length"] = {std::to_string(body.size())};

    for (auto& h : headers) {
        req.headers[h.first] = {h.second};
    }

    return execute_request(req, url.host, url.port);
}

HTTPResponse HTTPClient::http_put(const std::string& url_str,
                                  const std::unordered_map<std::string, std::string>& headers,
                                  const std::string& body) {

    URL url = parse_url(url_str);

    if (url.scheme != "http") {
        throw std::runtime_error("Only HTTP supported");
    }

    HTTPRequest req;
    req.method = "PUT";
    req.request_target = build_target(url);
    req.version = "HTTP/1.1";
    req.body = body;

    req.headers["host"] = {url.host};
    req.headers["connection"] = {"close"};
    req.headers["content-length"] = {std::to_string(body.size())};

    for (auto& h : headers) {
        req.headers[h.first] = {h.second};
    }

    return execute_request(req, url.host, url.port);
}

HTTPResponse HTTPClient::http_delete(const std::string& url_str,
                                     const std::unordered_map<std::string, std::string>& headers,
                                     const std::string& body) {

    URL url = parse_url(url_str);

    if (url.scheme != "http") {
        throw std::runtime_error("Only HTTP supported");
    }

    HTTPRequest req;
    req.method = "DELETE";
    req.request_target = build_target(url);
    req.version = "HTTP/1.1";
    req.body = body;

    req.headers["host"] = {url.host};
    req.headers["connection"] = {"close"};

    if (!body.empty()) {
        req.headers["content-length"] = {std::to_string(body.size())};
    }

    for (auto& h : headers) {
        req.headers[h.first] = {h.second};
    }

    return execute_request(req, url.host, url.port);
}
