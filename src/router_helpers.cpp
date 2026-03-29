#include "router_helpers.hpp"

#include <stdexcept>

void validate_path(const std::string& path) {
    if (path.empty() || path[0] != '/') {
        throw std::invalid_argument("Invalid route path: " + path + " (must start with '/')");
    }
}

std::string normalize_path(const std::string& path) {
    std::string normalized_path = path;
    while (normalized_path.size() > 1 && normalized_path.back() == '/') {
        path.pop_back();
    }
    return normalized_path;
}

bool match_route(const std::string& route_path, const std::string& request_path, HTTPRequest& request) {
    std::stringstream route_stream(route_path), request_stream(request_path);
    std::string route_segment, request_segment;

    while (getline(route_stream, route_segment, '/') && getline(request_stream, request_segment, '/')) {
        if (!route_segment.empty() && route_segment[0] == ':') {
            // Dynamic parameter
            request.parameters[route_segment.substr(1)] = request_segment;
        } else if (route_segment != request_segment) {
            return false;
        }
    }

    return route_stream.eof() && request_stream.eof();
}

std::string get_current_date() {
    time_t now = std::time(nullptr);
    tm gm_time{};
    gmtime_r(&now, &gm_time);

    char buf[30];
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &gm_time);
    return std::string(buf);
}
