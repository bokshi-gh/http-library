#include "http_server_helpers.hpp"

std::string get_current_date() {
    time_t now = std::time(nullptr);
    tm gm_time{};
    gmtime_r(&now, &gm_time);

    char buf[30];
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", &gm_time);
    return std::string(buf);
}
