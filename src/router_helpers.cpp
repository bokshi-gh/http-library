#include "router_helpers.hpp"

bool match_pattern(const std::string& pattern, const std::string& path, HTTPRequest& request) {
    std::stringstream p(pattern), q(path);
    std::string pp, qq;

    while (getline(p, pp, '/') && getline(q, qq, '/')) {
        if (!pp.empty() && pp[0] == ':') {
            request.parameters[pp.substr(1)] = qq;
        } else if (pp != qq) {
            return false;
        }
    }

    return p.eof() && q.eof();
}
