#pragma once

#include "http_client.hpp"
#include "http_client_helpers.hpp"

#include <string>
#include <cctype>

URL parse_url(const std::string& url);
std::string build_target(const URL& url);
