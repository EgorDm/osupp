//
// Created by egordm on 26-8-2017.
//

#ifndef OSUPP_BEATMAP_H
#define OSUPP_BEATMAP_H

#include "models.h"

class BeatmapReader {
private:

    template<typename T>
    static void parseAttrib(std::map<std::string, std::string> &attribs, std::string key, T &target);

    template<typename T>
    static void parse(std::string value, T &target);

    static void parse(std::string value, std::string &target);
};


#endif //OSUPP_BEATMAP_H
