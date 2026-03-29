#pragma once

#include <string>
#include <sstream>
#include "http_codec.hpp"

// Returns true if the route_path matches the request_path
// Fills request.parameters for dynamic segments like :id
bool match_route(const std::string& route_path, const std::string& request_path, HTTPRequest& request);
