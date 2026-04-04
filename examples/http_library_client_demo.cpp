#include "http_client.hpp"
#include <iostream>

void print_response(const HTTPResponse& res) {
    std::cout << "Status: " << res.status_code << "\n";
    std::cout << "Reason: " << res.reason_phrase << "\n";

    std::cout << "\n--- Headers ---\n";
    for (const auto& h : res.headers) {
        std::cout << h.first << ": ";
        for (const auto& v : h.second) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n--- Body ---\n";
    std::cout << res.body << "\n";
    std::cout << "\n===========================\n\n";
}

int main() {
    try {
        // ----------- GET -----------
        std::cout << "===== GET =====\n";
        HTTPResponse get_res = HTTPClient::http_get(
            "http://example.com/",
            {}
        );
        print_response(get_res);

        // ----------- POST -----------
        std::cout << "===== POST =====\n";
        HTTPResponse post_res = HTTPClient::http_post(
            "http://httpbin.org/post",
            {{"content-type", "application/json"}},
            R"({"name":"bokshi","type":"test"})"
        );
        print_response(post_res);

        // ----------- PUT -----------
        std::cout << "===== PUT =====\n";
        HTTPResponse put_res = HTTPClient::http_put(
            "http://httpbin.org/put",
            {{"content-type", "application/json"}},
            R"({"update":"true"})"
        );
        print_response(put_res);

        // ----------- DELETE -----------
        std::cout << "===== DELETE =====\n";
        HTTPResponse del_res = HTTPClient::http_delete(
            "http://httpbin.org/delete",
            {},
            ""
        );
        print_response(del_res);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
