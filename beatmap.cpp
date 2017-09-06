//
// Created by egordm on 26-8-2017.
//

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include "beatmap.h"
#include "utils.h"

Beatmap BeatmapReader::read(std::string path) {
    auto ret = Beatmap();
    std::ifstream file(path);

    readMeta(file, ret);
    readDifficulty(file, ret);
    readTimingPoints(file, ret);
    readHitObjects(file, ret);
    return ret;
}

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
        parse(tokens.at(0), offset);
        parse(tokens.at(1), mpb);
        if (mpb > 0) {
            lastKeyPoint = std::make_shared<KeyTimingPoint>(offset, mpb);
            beatmap.timingpoints.push_back(lastKeyPoint);
        } else {
            if(lastKeyPoint == nullptr) throw std::runtime_error("Beatmap starts with an inherited timing point.");
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
            pos = Coordinate(std::stof(tokens.at(0)), std::stof(tokens.at(1)));
            time = std::stoi(tokens.at(2));
            type = std::stoi(tokens.at(3));
            if ((type & HitCircle::HitObjectType::HitCircle) != 0) {
                beatmap.hitobjects.push_back(std::make_shared<HitCircle>(pos, time));
            } else if ((type & HitCircle::HitObjectType::Spinner) != 0) {
                beatmap.hitobjects.push_back(std::make_shared<Spinner>(pos, time, std::stoi(tokens.at(5))));
            } else if ((type & HitCircle::HitObjectType::Slider) != 0) {
                float pxLength = std::stof(tokens.at(7));
                if(pxLength == 0) {
                    beatmap.hitobjects.push_back(std::make_shared<HitCircle>(pos, time));
                    continue;
                }
                Curve curve = parseCurve(pos, tokens.at(5), pxLength);
                beatmap.hitobjects.push_back(
                        std::make_shared<Slider>(pos, time, std::stoi(tokens.at(6)), pxLength, curve));
            }
        } catch (...) {
            // TODO:  something
        }
    }
}

Curve BeatmapReader::parseCurve(Coordinate pos, std::string &str, const float &pxLength) {
    auto tokens = utils::split(str, "|", -1);

    std::vector<Coordinate> points{pos};
    auto type = static_cast<Curve::CurveType>(tokens.at(0).at(0));
    for (int i = 1; i < tokens.size(); ++i) {
        auto coordp = utils::split(tokens[i], ":", 2);
        points.emplace_back(std::stof(coordp.at(0)), std::stof(coordp.at(1)));
    }
    return Curve(type, points, pxLength);
}

std::vector<std::string> BeatmapReader::readSection(std::ifstream &file, std::string sectionTag) {
    std::vector<std::string> ret;
    std::string line;

    while (utils::getline(file, line) && line != sectionTag) continue;
    if(file.eof()) throw std::runtime_error("Section not found: " + sectionTag);
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

