#pragma once

#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <cstdlib>

namespace dotenv {

    static std::unordered_map<std::string, std::string> envmap;

    inline std::string trim(std::string s) {
        s.erase(0, s.find_first_not_of(" \t\r\n"));
        s.erase(s.find_last_not_of(" \t\r\n") + 1);
        return s;
    }

    inline void init(const char* path = nullptr) {
        const char* filepath = path ? path : "./.env";

        std::ifstream file(filepath);
        if (!file.is_open()) return;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            auto pos = line.find('=');
            if (pos == std::string::npos) continue;

            std::string key = trim(line.substr(0, pos));
            std::string value = trim(line.substr(pos + 1));

            // remove quotes
            if ((value.front() == '"' && value.back() == '"') ||
                (value.front() == '\'' && value.back() == '\'')) {
                value = value.substr(1, value.size() - 2);
            }

            envmap[key] = value;
        }
    }

    inline std::string getenv(const std::string& key) {
        if (envmap.find(key) != envmap.end())
            return envmap[key];
        else {
            std::cerr << "FATAL: Environment variable '" << key << "' not found!" << "\n";
            std::exit(EXIT_FAILURE);
        }
    }
}
