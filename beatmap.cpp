//
// Created by egordm on 26-8-2017.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include "beatmap.h"
#include "utils.h"

void BeatmapReader::readMeta(std::ifstream &file, Beatmap &beatmap) {
    auto section = readAttributeSection(file, "[Metadata]");
    parseAttrib(section, "Title", beatmap.title);
    parseAttrib(section, "Artist", beatmap.artist);
    parseAttrib(section, "Creator", beatmap.creator);
    parseAttrib(section, "Version", beatmap.version);
    parseAttrib(section, "BeatmapID", beatmap.id);
    parseAttrib(section, "BeatmapSetID", beatmap.set_id);
}

void BeatmapReader::readDifficulty(std::ifstream &file, Beatmap &beatmap) {
    auto section = readAttributeSection(file, "[Difficulty]");
    parseAttrib(section, "HPDrainRate", beatmap.hp);
    parseAttrib(section, "CircleSize", beatmap.cs);
    parseAttrib(section, "OverallDifficulty", beatmap.od);
    parseAttrib(section, "ApproachRate", beatmap.ar);
    parseAttrib(section, "SliderMultiplier", beatmap.sliderMultiplayer);
    parseAttrib(section, "SliderTickRate", beatmap.sliderTickRate);
}

void BeatmapReader::readTimingPoints(std::ifstream &file, Beatmap &beatmap) {
    int offset;
    float mpb;
    std::shared_ptr<KeyTimingPoint> lastKeyPoint = nullptr;
    for (const auto &line : readSection(file, "[TimingPoints]")) {
        auto tokens = utils::split(line, ",", 3);
        parse(tokens[0], offset);
        parse(tokens[1], mpb);
        if (mpb > 0) {
            lastKeyPoint = std::make_shared<KeyTimingPoint>(offset, mpb);
            beatmap.timingpoints.push_back(lastKeyPoint);
        } else {
            auto tp = std::make_shared<InheritedTimingPoint>(offset, mpb / -100);
            tp->parent = lastKeyPoint;
            beatmap.timingpoints.push_back(tp);
        }
    }
}

void BeatmapReader::readHitObjects(std::ifstream &file, Beatmap &beatmap) {
    Coordinate pos;
    int time;
    int type;
    for (const auto &line : readSection(file, "[HitObjects]")) {
        try {
            auto tokens = utils::split(line, ",", -1);
            pos = Coordinate(std::stof(tokens[0]), std::stof(tokens[1]));

            time = std::stoi(tokens[2]);
            type = std::stoi(tokens[3]);
            if ((type & HitCircle::HitObjectType::HitCircle) != 0) {
                beatmap.hitobjects.push_back(std::make_shared<HitCircle>(pos, time));
            } else if ((type & HitCircle::HitObjectType::Spinner) != 0) {
                beatmap.hitobjects.push_back(std::make_shared<Spinner>(pos, time, std::stoi(tokens[5])));
            } else if ((type & HitCircle::HitObjectType::Slider) != 0) {
                float pxLength = std::stof(tokens[7]);
                Curve curve = parseCurve(pos, tokens[5], pxLength);
                beatmap.hitobjects.push_back(
                        std::make_shared<Slider>(pos, time, std::stoi(tokens[6]), pxLength, curve));
            }
        } catch (...) {
            // TODO:  something
        }
    }
}
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

