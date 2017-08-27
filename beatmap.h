//
// Created by egordm on 26-8-2017.
//

#ifndef OSUPP_BEATMAP_H
#define OSUPP_BEATMAP_H

#include "models.h"

class BeatmapReader {
private:
    static void readMeta(std::ifstream &file, Beatmap &beatmap);

    static void readDifficulty(std::ifstream &file, Beatmap &beatmap);

    static void readTimingPoints(std::ifstream &file, Beatmap &beatmap);

    static void readHitObjects(std::ifstream &file, Beatmap &beatmap);

    static Curve parseCurve(Coordinate pos, std::string &str, const float &pxLength);

    static std::vector<std::string> readSection(std::ifstream &file, std::string sectionTag);

    static std::map<std::string, std::string> readAttributeSection(std::ifstream &file, std::string sectionTag);

    template<typename T>
    static void parseAttrib(std::map<std::string, std::string> &attribs, std::string key, T &target);

    template<typename T>
    static void parse(std::string value, T &target);

    static void parse(std::string value, std::string &target);
};


#endif //OSUPP_BEATMAP_H
