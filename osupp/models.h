//
// Created by egordm on 24-8-2017.
//

#ifndef OSUPP_MODELS_H
#define OSUPP_MODELS_H


#include <vector>
#include <map>
#include <memory>
#include "curves.h"


namespace osupp {
    struct TimingPoint {
        double offset = 0;
        float slider_multiplayer = 1;
        unsigned int meter = 4;

        TimingPoint() = default;

        explicit TimingPoint(double offset) : offset(offset) {}

        virtual double get_mpb() { return -1; };
    };

    struct KeyTimingPoint : TimingPoint {
        double mpb = 1;

        KeyTimingPoint() = default;

        KeyTimingPoint(double offset, double mpb) : TimingPoint(offset), mpb(mpb) {}

        double get_mpb() override {
            return mpb;
        }
    };

    struct InheritedTimingPoint : TimingPoint {
        KeyTimingPoint* parent; // TODO: this is good and bad. If beatmap is deleted  -> rip

        InheritedTimingPoint() = default;

        InheritedTimingPoint(double offset, float slider_multiplayer) : TimingPoint(
                offset) { this->slider_multiplayer = slider_multiplayer; }

        double get_mpb() override {
            return parent->get_mpb();
        }
    };

    struct HitObject {
        enum HitObjectType {
            Unknown = 0,
            HitCircle = 1,
            Slider = 2,
            NewCombo = 4,
            Spinner = 8
        };

        HitObject(const Coordinate &pos, unsigned long time) : pos(pos), time(time) {}

        virtual HitObjectType get_type() { return HitObjectType::Unknown; }

        Coordinate pos;
        unsigned long time;
        bool new_combo = false;
    };

    struct HitCircle : HitObject {
        HitCircle(const Coordinate &pos, unsigned long time) : HitObject(pos, time) {}

        HitObjectType get_type() override {
            return HitObjectType::HitCircle;
        }
    };

    struct Slider : HitObject {
        Slider(const Coordinate &pos, unsigned long time, int repeat, float pixel_length, Curve &curve)
                : HitObject(pos, time), repeat(repeat), pixel_length(pixel_length), curve(curve) {}

        HitObjectType get_type() override {
            return HitObjectType::Slider;
        }

        unsigned long get_slider_duration(float sliderMultiplayer, TimingPoint *tp);

        bool in_slider(unsigned long t, float sliderMultiplayer, TimingPoint *tp);

        Coordinate pos_at(unsigned long t, float sliderMultiplayer, TimingPoint *tp);

        int repeat;
        float pixel_length;
        Curve curve;
    };

    struct Spinner : HitObject {
        Spinner(const Coordinate &pos, unsigned long time, unsigned long end_time) : HitObject(pos, time),
                                                                                    end_time(end_time) {};

        HitObjectType get_type() override {
            return HitObjectType::Spinner;
        }

        unsigned long end_time;
    };

    struct BeatmapEntry {
        std::string title;
        std::string artist;
        std::string creator;
        std::string version;
        std::string audio_file;
        std::string osu_file;
        std::string folder_name;

        char mode;
        char ranked;

        int id;
        int set_id;

        float ar;
        float cs;
        float hp;
        float od;

        std::map<int, double> std_diffs;
        std::map<int, double> taiko_diffs;
        std::map<int, double> ctb_diffs;
        std::map<int, double> mania_diffs;

        int time_drain;
        int time_total;

        std::vector<std::shared_ptr<TimingPoint>> timingpoints;

        std::string get_path(std::string osu_root);
    };

    struct Beatmap {
        std::string title;
        std::string artist;
        std::string creator;
        std::string version;

        int id;
        int set_id;

        float ar;
        float cs;
        float hp;
        float od;

        float slider_multiplayer;
        float slider_tick_rate;

        std::vector<std::shared_ptr<TimingPoint>> timingpoints;
        std::vector<std::shared_ptr<HitObject>> hitobjects;
    };
}

#endif //OSUPP_MODELS_H
