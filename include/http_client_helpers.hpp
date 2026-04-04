#pragma once

#include "http_client.hpp"

#include <string>
#include <cctype>
#include <stdexcept>

URL parse_url(const std::string& url);
std::string build_target(const URL& url);
