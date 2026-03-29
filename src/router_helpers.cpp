#include "router_helpers.hpp"

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
