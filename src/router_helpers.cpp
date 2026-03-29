#include <functional>

struct RouteHash {
    std::size_t operator()(const Route& r) const {
        std::size_t h1 = std::hash<std::string>{}(r.method);
        std::size_t h2 = std::hash<std::string>{}(r.path);
        return h1 ^ (h2 * 31); // combine hashes
    }
};
