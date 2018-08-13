//
// Created by egordm on 9-10-2017.
//

#ifndef OSUPP_BEATMAP_IO_H
#define OSUPP_BEATMAP_IO_H


#include <istream>
#include <map>
#include <vector>
#include <sstream>
#include "models.h"
#include "utils.h"

namespace osupp {
    class BeatmapReader {
    public:
        explicit BeatmapReader(std::istream &is);

        long find_section(const std::string &sectionTag);

        long next_section(std::string &sectionTag);

        std::vector<std::string> readSection(const std::string &sectionTag);

        std::map<std::string, std::string> read_attribute_section(const std::string &sectionTag);

    private:
        std::istream &is;
    };

    class BeatmapWriter {
    public:
        explicit BeatmapWriter(std::ostream &os);

        void writeSection(const std::string &sectionTag, const std::vector<std::string> &content);

    private:
        std::ostream &os;
    };

    namespace reading {
        template<typename T>
        inline void parse(const std::string &value, T &target) {
            std::stringstream ss(value);
            ss >> target;
        }

        inline void parse(const std::string &value, std::string &target) {
            target = value;
        }

        void parse(const std::string &value, std::shared_ptr<TimingPoint> &target);

        Curve parse(const std::string &value, const Coordinate &pos, const float &px_length);

        inline void parse(const std::string &value, std::shared_ptr<HitObject> &target);

        template<typename T>
        inline void parse_attrib(std::map<std::string, std::string> &attribs, std::string key, T &target) {
            auto it = attribs.find(key);
            if (it == attribs.end()) return;
            try {
                parse(it->second, target);
            } catch (int e) {}
        }
    }
}

#endif //OSUPP_BEATMAP_IO_H
