//
// Created by egordm on 11-9-2017.
//

#include <sstream>
#include "beatmap_file.h"
#include "utils.h"

namespace osupp {
    BeatmapFile::BeatmapFile(const std::string &path, BeatmapFile::Mode mode) : path(path), mode(mode) {
        setMode(mode);
    }

    BeatmapFile::Mode BeatmapFile::getMode() const {
        return mode;
    }

    void BeatmapFile::setMode(BeatmapFile::Mode mode) {
        if (file.is_open()) file.close();
        file.open(path, mode == Mode::Read ? std::ios::in : std::ios::out);
        if (!file.good()) throw std::runtime_error("Beatmap not found: " + path);
        BeatmapFile::mode = mode;
    }

    long BeatmapFile::findSection(const std::string &sectionTag) {
        file.seekg(std::ios::beg);
        std::string line;
        while (utils::getline(file, line) && !file.eof() && line != sectionTag) continue;
        return file.eof() ? std::ios::end : file.tellg();
    }

    long BeatmapFile::nextSection(std::string &sectionTag) {
        std::string line;
        while (utils::getline(file, line) && !line.empty() && line.at(0) != '[' && line.at(line.length() - 1) != ']');
        sectionTag = line;
        return file.eof() ? std::ios::end : file.tellg();
    }

    std::vector<std::string> BeatmapFile::readSection(const std::string &sectionTag) {
        if (findSection(sectionTag) == std::ios::end) throw std::runtime_error("Section not found: " + sectionTag);
        std::vector<std::string> ret;
        std::string line;

        while (utils::getline(file, line) && !line.empty()) ret.push_back(line);
        return ret;
    }

    std::map<std::string, std::string> BeatmapFile::readAttributeSection(const std::string &sectionTag) {
        std::map<std::string, std::string> ret;
        for (const auto &line : readSection(sectionTag)) {
            unsigned long pos = line.find(':');
            if (pos != std::string::npos)
                ret[line.substr(0, pos)] = line.substr(pos + 1, line.size() - pos);
        }
        return ret;
    }

    void BeatmapFile::writeSection(const std::string &sectionTag, const std::vector<std::string> &content) {
        if (getMode() != Mode::Write) throw std::runtime_error("File is not in write mode.");
        file << std::endl << sectionTag << std::endl;
        for (const auto &line : content) file << line << std::endl;
    }
}
