#include "http_client_helpers.hpp"

URL parse_url(const std::string& url) {
    URL result;

    size_t i = 0;
    size_t n = url.size();

    //  1. Scheme (optional)
    size_t scheme_end = url.find("://");
    if (scheme_end != std::string::npos) {
        result.scheme = url.substr(0, scheme_end);
        i = scheme_end + 3;
    }

    //  2. Host (IPv6 or normal)
    if (i < n && url[i] == '[') {
        // IPv6: [::1]
        i++; // skip '['
        size_t host_start = i;

        while (i < n && url[i] != ']') i++;

        result.host = url.substr(host_start, i - host_start);

        if (i < n && url[i] == ']') i++; // skip ']'
    } else {
        // Normal host / IPv4 / domain
        size_t host_start = i;

        while (i < n && url[i] != ':' && url[i] != '/' && url[i] != '?') i++;

        result.host = url.substr(host_start, i - host_start);
    }

    //  3. Port (optional)
    if (i < n && url[i] == ':') {
        i++; // skip ':'
        size_t port_start = i;

        while (i < n && std::isdigit(url[i])) i++;

        result.port = url.substr(port_start, i - port_start);
    }

    //  4. Path
    if (i < n && url[i] == '/') {
        size_t path_start = i;

        while (i < n && url[i] != '?') i++;

        result.path = url.substr(path_start, i - path_start);
    } else {
        result.path = "/"; // default
    }

    //  5. Query
    if (i < n && url[i] == '?') {
        i++; // skip '?'
        result.query = url.substr(i);
    }

    return result;
} 
