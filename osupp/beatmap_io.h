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

        long find_section(const std::string &section_tag);

        long next_section(std::string &section_tag);

        std::vector<std::string> read_section(const std::string &section_tag);

        std::map<std::string, std::string> read_attribute_section(const std::string &section_tag);

    private:
        std::istream &is;
    };

    class BeatmapWriter {
    public:
        explicit BeatmapWriter(std::ostream &os);

        void write_section(const std::string &sectionTag, const std::vector<std::string> &content);

    private:
        std::ostream &os;
    };

    const unsigned int META_SECTION = 1<<0;
    const unsigned int DIFFICULTY_SECTION = 1<<1;
    const unsigned int TIMING_SECTION = 1<<2;
    const unsigned int HITOBJECT_SECTION = 1<<3;
    const unsigned int ALL_SECTIONS = META_SECTION | DIFFICULTY_SECTION | TIMING_SECTION | HITOBJECT_SECTION;

    Beatmap read_beatmap(std::string file, unsigned int read_flags = ALL_SECTIONS);

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
