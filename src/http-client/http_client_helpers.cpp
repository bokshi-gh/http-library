#include "http_client_helpers.hpp"
#include <cctype>
#include <stdexcept>

URL parse_url(const std::string& url) {
    URL result;

    size_t i = 0;
    size_t n = url.size();

    // 1. Scheme (optional)
    size_t scheme_end = url.find("://");
    if (scheme_end != std::string::npos) {
        result.scheme = url.substr(0, scheme_end);
        i = scheme_end + 3;
    } else {
        result.scheme = "http";
    }

    // 2. Host
    if (i < n && url[i] == '[') {
        // IPv6
        i++;
        size_t start = i;
        while (i < n && url[i] != ']') i++;

        result.host = url.substr(start, i - start);

        if (i < n && url[i] == ']') i++;
    } else {
        size_t start = i;
        while (i < n && url[i] != ':' && url[i] != '/' && url[i] != '?') i++;

        result.host = url.substr(start, i - start);
    }

    if (result.host.empty()) {
        throw std::invalid_argument("Invalid URL: host missing");
    }

    // 3. Port
    if (i < n && url[i] == ':') {
        i++;
        size_t start = i;
        while (i < n && std::isdigit(url[i])) i++;

        result.port = url.substr(start, i - start);
    }

    // 4. Path
    if (i < n && url[i] == '/') {
        size_t start = i;
        while (i < n && url[i] != '?') i++;

        result.path = url.substr(start, i - start);
    } else {
        result.path = "/";
    }

    // 5. Query
    if (i < n && url[i] == '?') {
        i++;
        result.query = url.substr(i);
    }

    // 6. Default port
    if (result.port.empty()) {
        if (result.scheme == "http") result.port = "80";
        else if (result.scheme == "https") result.port = "443";
    }

    return result;
}

std::string build_target(const URL& url) {
    std::string target = url.path;

    if (!url.query.empty()) {
        target += "?" + url.query;
    }

    return target;
}
