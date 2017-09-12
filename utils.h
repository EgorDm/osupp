//
// Created by egordm on 26-8-2017.
//

#ifndef OSUPP_UTILS_H
#define OSUPP_UTILS_H

#include <vector>
#include <string>
#include <algorithm>

namespace utils {
    std::istream &getline(std::istream &is, std::string &t);

    inline int stoi(std::string value) {
        return std::stoi(value);
    }

    std::vector<std::string> split(const std::string &str, const std::string &delimeter, const int times);

    inline std::vector<std::string> split(const std::string &str, const std::string &delimeter) {
        split(str, delimeter, -1);
    };

    inline float clamp(const float &n, const float &lower, const float &upper) {
        return std::max(lower, std::min(n, upper));
    }
};


#endif //OSUPP_UTILS_H
