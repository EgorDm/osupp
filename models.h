//
// Created by egordm on 24-8-2017.
//

#ifndef OSUPP_MODELS_H
#define OSUPP_MODELS_H


#include <vector>
#include <map>
#include <memory>
#include "curves.h"


struct HitObject {
    enum HitObjectType {
        HitCircle = 1,
        Slider = 2,
        Spinner = 8
    };


    Coordinate pos;
    unsigned long time;

    HitObject(const Coordinate &pos, unsigned long time) : pos(pos), time(time) {}

};

struct HitCircle : HitObject {
    HitCircle(const Coordinate &pos, unsigned long time) : HitObject(pos, time) {}
};

struct Slider : HitObject {
    Slider(const Coordinate &pos, unsigned long time, int repeat, float pixelLength, Curve &curve)
            : HitObject(pos, time), repeat(repeat), pixelLength(pixelLength), curve(curve) {}

    int repeat;
    float pixelLength;
    Curve curve;
};

struct Spinner : HitObject {
    Spinner(const Coordinate &pos, unsigned long time, unsigned long endTime) : HitObject(pos, time), endTime(endTime) {};

    unsigned long endTime;
};

struct TimingPoint {
    double offset;

    TimingPoint() {}

    explicit TimingPoint(double offset) : offset(offset) {}
};

struct KeyTimingPoint : TimingPoint {
    double mpb;

    KeyTimingPoint() {}

    KeyTimingPoint(double offset, double mpb) : TimingPoint(offset), mpb(mpb) {}
};

struct InheritedTimingPoint : TimingPoint {
    std::shared_ptr<KeyTimingPoint> parent;
    float slider_multiplayer;

    InheritedTimingPoint() {}

    InheritedTimingPoint(double offset, float slider_multiplayer) : TimingPoint(offset),
                                                                    slider_multiplayer(slider_multiplayer) {}
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
};

};

#endif //OSUPP_MODELS_H
