//
// Created by egordm on 26-8-2017.
//

#include <iostream>
#include "utils.h"


namespace utils {
    std::istream &getline(std::istream &is, std::string &t) {
        t.clear();
        std::istream::sentry se(is, true);
        std::streambuf *sb = is.rdbuf();

        for (;;) {
            int c = sb->sbumpc();
            switch (c) {
                case '\n':
                    return is;
                case '\r':
                    if (sb->sgetc() == '\n')
                        sb->sbumpc();
                    return is;
                case EOF:
                    if (t.empty())
                        is.setstate(std::ios::eofbit);
                    return is;
                default:
                    t += (char) c;
            }
        }
    }

    std::vector<std::string> split(const std::string &str, const std::string &delimeter, const int times) {
        const auto pos = str.find(delimeter);
        if(pos == std::string::npos) return {str};

        std::vector<std::string> ret {str.substr(0, pos)};
        auto tail = split(str.substr(pos + delimeter.size(), std::string::npos), delimeter, times);
        ret.insert(ret.end(), tail.begin(), tail.end());

        return ret;
    }
}