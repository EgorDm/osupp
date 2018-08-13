//
// Created by egordm on 9-10-2017.
//

#include <fstream>
#include "beatmap_io.h"
#include "utils.h"

namespace osupp {
    BeatmapReader::BeatmapReader(std::istream &is) : is(is) {}

    long BeatmapReader::find_section(const std::string &section_tag) {
        is.seekg(std::ios::beg);
        std::string line;
        while (utils::getline(is, line) && !is.eof() && line != section_tag) continue;
        return is.eof() ? std::ios::end : is.tellg();
    }

    long BeatmapReader::next_section(std::string &section_tag) {
        std::string line;
        while (utils::getline(is, line) && !is.eof() && line.front() != '[' && line.back() != ']') continue;
        section_tag = line;
        return is.eof() ? std::ios::end : is.tellg();
    }

    std::vector<std::string> BeatmapReader::read_section(const std::string &section_tag) {
        if (find_section(section_tag) == std::ios::end) throw std::runtime_error("Section not found: " + section_tag);
        std::vector<std::string> ret;
        std::string line;

        while (utils::getline(is, line) && !line.empty()) ret.push_back(line);
        return ret;
    }

    std::map<std::string, std::string> BeatmapReader::read_attribute_section(const std::string &section_tag) {
        std::map<std::string, std::string> ret;
        for (const auto &line : read_section(section_tag)) {
            unsigned long pos = line.find(':');
            if (pos != std::string::npos)
                ret[line.substr(0, pos)] = line.substr(pos + 1, line.size() - pos);
        }
        return ret;
    }

    BeatmapWriter::BeatmapWriter(std::ostream &os) : os(os) {}

    void BeatmapWriter::write_section(const std::string &sectionTag, const std::vector<std::string> &content) {
        os << std::endl << sectionTag << std::endl;
        for (const auto &line : content) os << line << std::endl;
    }

    void reading::parse(const std::string &value, std::shared_ptr<TimingPoint> &target)  {
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

    Curve reading::parse(const std::string &value, const Coordinate &pos, const float &px_length)  {
        auto tokens = utils::split(value, "|", -1);
        std::vector<Coordinate> points{pos};
        auto type = static_cast<Curve::CurveType>(tokens.at(0).at(0));
        for (int i = 1; i < tokens.size(); ++i) {
            auto coordp = utils::split(tokens[i], ":", 2);
            points.emplace_back(std::stof(coordp.at(0)), std::stof(coordp.at(1)));
        }
        return Curve(type, points, px_length);
    }

    void reading::parse(const std::string &value, std::shared_ptr<HitObject> &target) {
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

    Beatmap read_beatmap(const std::string &file, unsigned int read_flags) {
        std::ifstream ifile(file); // TODO: close
        if(!ifile.is_open()) throw std::runtime_error("Could not open file " + file);

        Beatmap ret;

        try {
            BeatmapReader reader(ifile);
            if (read_flags & META_SECTION) {
                auto meta = reader.read_attribute_section("[Metadata]");
                ret.title = meta.at("Title");
                ret.artist = meta.at("Artist");
                ret.creator = meta.at("Creator");
                ret.version = meta.at("Version");
                ret.id = stoi(meta.at("BeatmapID"));
                ret.set_id = stoi(meta.at("BeatmapSetID"));
            }

            if (read_flags & DIFFICULTY_SECTION) {
                auto difficulty = reader.read_attribute_section("[Difficulty]");
                ret.hp = stof(difficulty.at("HPDrainRate"));
                ret.cs = stof(difficulty.at("CircleSize"));
                ret.od = stof(difficulty.at("OverallDifficulty"));
                ret.ar = stof(difficulty.at("ApproachRate"));
                ret.slider_multiplayer = stof(difficulty.at("SliderMultiplier"));
                ret.slider_tick_rate = stof(difficulty.at("SliderTickRate"));
            }

            if (read_flags & TIMING_SECTION) {
                auto timings = reader.read_section("[TimingPoints]");
                KeyTimingPoint *ktp = nullptr;

                for (int i = 0; i < timings.size(); ++i) {
                    std::shared_ptr<TimingPoint> t;
                    reading::parse(timings[i], t);

                    auto *tkp = dynamic_cast<KeyTimingPoint *>(t.get());
                    if (tkp) ktp = tkp;

                    auto *itp = dynamic_cast<InheritedTimingPoint *>(t.get());
                    if (itp) itp->parent = ktp;

                    ret.timingpoints.push_back(t);
                }
            }

            if (read_flags & HITOBJECT_SECTION) {
                auto hitobjects = reader.read_section("[HitObjects]");
                for (const std::string &v : hitobjects) {
                    std::shared_ptr<HitObject> t;
                    reading::parse(v, t);
                    ret.hitobjects.push_back(t);
                }
            }
        } catch (...) {}

        ifile.close();
        return ret;
    }
}