#pragma once

#include "http_codec.hpp"

#include <string>
#include <sstream>
#include <ctime>

void validate_path(const std::string& path);
string normalize_path(const std::string& path);

// Returns true if the route_path matches the request_path
// Fills request.parameters for dynamic segments like :id
bool match_route(const std::string& route_path, const std::string& request_path, HTTPRequest& request);

std::string get_current_date();
