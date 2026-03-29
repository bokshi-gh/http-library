#pragma once

#include <string>
#include <sstream>
#include "http_codec.hpp"

// Returns true if pattern matches path, fills request.parameters for :params
bool match_pattern(const std::string& pattern, const std::string& path, HTTPRequest& request);
