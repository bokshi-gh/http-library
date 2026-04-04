#include "router_helpers.hpp"

#include <stdexcept>

bool match_route(const std::string& route_path,
                 const std::string& request_path,
                 HTTPRequest& request) {

    // Special case: root must match only root
    if (route_path == "/") {
        return request_path == "/";
    }

    std::vector<std::string> route_parts;
    std::vector<std::string> request_parts;

    std::stringstream rs(route_path), qs(request_path);
    std::string segment;

    while (getline(rs, segment, '/')) {
        if (!segment.empty()) route_parts.push_back(segment);
    }

    while (getline(qs, segment, '/')) {
        if (!segment.empty()) request_parts.push_back(segment);
    }

    // Critical: must have same number of segments
    if (route_parts.size() != request_parts.size()) {
        return false;
    }

    for (size_t i = 0; i < route_parts.size(); i++) {
        const auto& route_seg = route_parts[i];
        const auto& req_seg = request_parts[i];

        if (!route_seg.empty() && route_seg[0] == ':') {
            request.parameters[route_seg.substr(1)] = req_seg;
        } else if (route_seg != req_seg) {
            return false;
        }
    }

    return true;
}
