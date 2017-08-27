//
// Created by egordm on 26-8-2017.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include "beatmap.h"
#include "utils.h"

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

