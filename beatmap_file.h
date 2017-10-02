//
// Created by egordm on 11-9-2017.
//

#ifndef OSUPP_BEATMAP_FILE_H
#define OSUPP_BEATMAP_FILE_H

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include "curves.h"
#include "models.h"
#include "utils.h"

namespace osupp {
    class BeatmapFile {
    public:
        enum Mode {
            Read, Write
        };

        explicit BeatmapFile(const std::string &path, Mode mode);

        long findSection(const std::string &sectionTag);

        long nextSection(std::string &sectionTag);

        std::vector<std::string> readSection(const std::string &sectionTag);

        std::map<std::string, std::string> readAttributeSection(const std::string &sectionTag);

        void writeSection(const std::string &sectionTag, const std::vector<std::string> &content);

        Mode getMode() const;

        void setMode(Mode mode);

        inline void close() { file.close(); }

    private:
        std::string path;
        Mode mode;
        std::fstream file;
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

        inline void parse(const std::string &value, std::shared_ptr<TimingPoint> &target) {
            int offset; // TODO: bit inconsistent are we?
            float mpb;
            unsigned int meter = 4;
            auto tokens = utils::split(value, ",", 4);
            parse(tokens.at(0), offset);
            parse(tokens.at(1), mpb);
            if(tokens.size() > 3) parse(tokens.at(2), meter);
            if (mpb > 0) target = std::make_shared<KeyTimingPoint>(offset, mpb);
            else target = std::make_shared<InheritedTimingPoint>(offset, -100 / mpb);
            target->meter = meter;
        }

        inline Curve parse(const std::string &value, const Coordinate &pos, const float &pxLength) {
            auto tokens = utils::split(value, "|", -1);
            std::vector<Coordinate> points{pos};
            auto type = static_cast<Curve::CurveType>(tokens.at(0).at(0));
            for (int i = 1; i < tokens.size(); ++i) {
                auto coordp = utils::split(tokens[i], ":", 2);
                points.emplace_back(std::stof(coordp.at(0)), std::stof(coordp.at(1)));
            }
            return Curve(type, points, pxLength);
        }

        inline void parse(const std::string &value, std::shared_ptr<HitObject> &target) {
            Coordinate pos;
            int time;
            int type;
            auto tokens = utils::split(value, ",", -1);
            pos = Coordinate(std::stof(tokens.at(0)), std::stof(tokens.at(1)));
            time = std::stoi(tokens.at(2));
            type = std::stoi(tokens.at(3));
            if ((type & HitCircle::HitObjectType::HitCircle) != 0)
                target = std::make_shared<HitCircle>(pos, time);
            else if ((type & HitCircle::HitObjectType::Spinner) != 0)
                target = std::make_shared<Spinner>(pos, time, std::stoi(tokens.at(5)));
            else if ((type & HitCircle::HitObjectType::Slider) != 0) {
                float pxLength = std::stof(tokens.at(7));
                if (pxLength == 0) {
                    target = std::make_shared<HitCircle>(pos, time);
                    throw new std::runtime_error("Slider has no length!");
                }
                Curve curve = parse(tokens.at(5), pos, pxLength);
                target = std::make_shared<Slider>(pos, time, std::stoi(tokens.at(6)), pxLength, curve);
            }
            target->new_combo = (type & HitCircle::HitObjectType::NewCombo) != 0;
        }

        template<typename T>
        inline void parseAttrib(std::map<std::string, std::string> &attribs, std::string key, T &target) {
            auto it = attribs.find(key);
            if (it == attribs.end()) return;
            try {
                parse(it->second, target);
            } catch (int e) {}
        }
    }
}


#endif //OSUPP_BEATMAP_FILE_H
