//
// Created by egordm on 9-10-2017.
//

#include "beatmap_io.h"
#include "utils.h"

namespace osupp {
    BeatmapReader::BeatmapReader(std::istream &is) : is(is) {}

    long BeatmapReader::findSection(const std::string &sectionTag) {
        is.seekg(std::ios::beg);
        std::string line;
        while (utils::getline(is, line) && !is.eof() && line != sectionTag) continue;
        return is.eof() ? std::ios::end : is.tellg();
    }

    long BeatmapReader::nextSection(std::string &sectionTag) {
        std::string line;
        while (utils::getline(is, line) && !is.eof() && line.front() != '[' && line.back() != ']') continue;
        sectionTag = line;
        return is.eof() ? std::ios::end : is.tellg();
    }

    std::vector<std::string> BeatmapReader::readSection(const std::string &sectionTag) {
        if (findSection(sectionTag) == std::ios::end) throw std::runtime_error("Section not found: " + sectionTag);
        std::vector<std::string> ret;
        std::string line;

        while (utils::getline(is, line) && !line.empty()) ret.push_back(line);
        return ret;
    }

    std::map<std::string, std::string> BeatmapReader::readAttributeSection(const std::string &sectionTag) {
        std::map<std::string, std::string> ret;
        for (const auto &line : readSection(sectionTag)) {
            unsigned long pos = line.find(':');
            if (pos != std::string::npos)
                ret[line.substr(0, pos)] = line.substr(pos + 1, line.size() - pos);
        }
        return ret;
    }

    BeatmapWriter::BeatmapWriter(std::ostream &os) : os(os) {}

    void BeatmapWriter::writeSection(const std::string &sectionTag, const std::vector<std::string> &content) {
        os << std::endl << sectionTag << std::endl;
        for (const auto &line : content) os << line << std::endl;
    }
}