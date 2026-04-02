#include "http_client.hpp"

HTTPResponse HTTPClient::http_get(const string& url, const unordered_map<string, string>& headers) {
     
}

int main() {
    const char* host = "example.com";  // raw host
    const char* port = "80";           // raw port

    // check schmea if none make http if https error not suported if other err invalid schmea

    // 1. Resolve host
    struct addrinfo hints{}, *res;
    hints.ai_family = AF_UNSPEC;       // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host, port, &hints, &res) != 0) {
        std::cerr << "Failed to resolve host\n";
        return 1;
    }

    // 2. Create socket
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) {
        std::cerr << "Socket creation failed\n";
        freeaddrinfo(res);
        return 1;
    }

    // 3. Connect
    if (::connect(sockfd, res->ai_addr, res->ai_addrlen) != 0) {
        std::cerr << "Connection failed\n";
        close(sockfd);
        freeaddrinfo(res);
        return 1;
    }

    freeaddrinfo(res);

    // 4. Raw HTTP GET request (hardcoded path)
    const char* request =
        "GET / HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Connection: close\r\n\r\n";

    send(sockfd, request, strlen(request), 0);

    // 5. Receive raw response
    char buffer[4096];
    int bytes;
    while ((bytes = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        std::cout.write(buffer, bytes);
    }

    close(sockfd);
    return 0;
}
