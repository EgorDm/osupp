//
// Created by egordm on 26-8-2017.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include "beatmap.h"
#include "utils.h"

std::vector<std::string> BeatmapReader::readSection(std::ifstream &file, std::string sectionTag) {
    std::vector<std::string> ret;
    std::string line;

    while (utils::getline(file, line) && line != sectionTag) continue;
    while (utils::getline(file, line) && !line.empty()) {
        ret.push_back(line);
    }
    return ret;
}

std::map<std::string, std::string> BeatmapReader::readAttributeSection(std::ifstream &file, std::string sectionTag) {
    std::map<std::string, std::string> ret;
    for (const auto &line : readSection(file, sectionTag)) {
        unsigned long pos = line.find(':');
        if (pos != std::string::npos)
            ret[line.substr(0, pos)] = line.substr(pos + 1, line.size() - pos);
    }
    return ret;
}

template<typename T>
void BeatmapReader::parseAttrib(std::map<std::string, std::string> &attribs, std::string key, T &target) {
    auto it = attribs.find(key);
    if (it == attribs.end()) return;
    try {
        parse(it->second, target);

    } catch (int e) {}
}

template<typename T>
void BeatmapReader::parse(std::string value, T &target) {
    std::stringstream ss(value);
    ss >> target;
}

void BeatmapReader::parse(std::string value, std::string &target) {
    target = value;
}

