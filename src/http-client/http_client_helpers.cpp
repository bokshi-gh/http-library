#include "http_client_helpers.hpp"

URL parse_url(const std::string& url) {
    ParsedURL result;

    size_t i = 0;
    size_t n = url.size();

    // 🔹 1. Parse scheme (optional)
    size_t scheme_end = url.find("://");
    if (scheme_end != std::string::npos) {
        result.scheme = url.substr(0, scheme_end);
        i = scheme_end + 3;
    }

    // 🔹 2. Parse host (domain or IP)
    size_t host_start = i;
    while (i < n && url[i] != ':' && url[i] != '/' && url[i] != '?') {
        i++;
    }
    result.host = url.substr(host_start, i - host_start);

    // 🔹 3. Parse port (optional)
    if (i < n && url[i] == ':') {
        i++; // skip ':'
        size_t port_start = i;
        while (i < n && isdigit(url[i])) {
            i++;
        }
        result.port = url.substr(port_start, i - port_start);
    }

    // 🔹 4. Parse path
    if (i < n && url[i] == '/') {
        size_t path_start = i;
        while (i < n && url[i] != '?') {
            i++;
        }
        result.path = url.substr(path_start, i - path_start);
    } else {
        result.path = "/"; // default
    }

    // 🔹 5. Parse query (optional)
    if (i < n && url[i] == '?') {
        i++; // skip '?'
        result.query = url.substr(i);
    }

    return result;
}
