#pragma once

#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

class Dotenv {
private:
    std::unordered_map<std::string, std::string> env;

    static std::string trim(const std::string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";

        size_t end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

public:
    Dotenv(const std::string& path = ".env") {
        std::ifstream file(path);
        if (!file) {
            std::cerr << "FATAL: could not open file '" << path << "'\n";
            std::exit(EXIT_FAILURE);
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            auto pos = line.find('=');
            if (pos == std::string::npos) continue;

            std::string key = trim(line.substr(0, pos));
            std::string val = trim(line.substr(pos + 1));

            if (val.size() >= 2 &&
                ((val.front() == '"' && val.back() == '"') ||
                 (val.front() == '\'' && val.back() == '\''))) {
                val = val.substr(1, val.size() - 2);
            }

            if (!key.empty())
                env[key] = val;
        }
    }

    std::string get(const std::string& key) const {
        auto it = env.find(key);
        if (it != env.end())
            return it->second;

        std::cerr << "FATAL: key '" << key << "' not found\n";
        std::exit(EXIT_FAILURE);
    }

    std::string get(const std::string& key, const std::string& def) const {
        auto it = env.find(key);
        return (it != env.end()) ? it->second : def;
    }

    std::size_t size() const {
        return env.size();
    }
};
