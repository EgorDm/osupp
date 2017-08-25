//
// Created by egordm on 24-8-2017.
//

#ifndef OSUPP_MODELS_H
#define OSUPP_MODELS_H


#include <vector>
#include <map>

struct HitObject {
    int x, y;
    unsigned long time;
};

struct HitCircle : HitObject {
    int sampleType;
    int number;
    int colorCounter;
    bool clicked;
};

struct Slider : HitObject {
    char type;
    int repeat;
    float pixelLength;
};

struct Spinner : HitObject {
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
    KeyTimingPoint *parent;
    float slider_multiplayer;

    InheritedTimingPoint() {}

    InheritedTimingPoint(double offset, float slider_multiplayer) : TimingPoint(offset),
                                                                    slider_multiplayer(slider_multiplayer) {}
};

class BeatmapEntry {
public:
    virtual ~BeatmapEntry();

    std::string title;
    std::string artist;
    std::string creator;
    std::string version;
    std::string audio_file;
    std::string osu_file;
    std::string folder_name;

    char mode;
    char ranked;

    int beatmap_id;
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

    std::vector<TimingPoint *> timingpoints;
};

#endif //OSUPP_MODELS_H
