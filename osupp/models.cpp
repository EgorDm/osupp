//
// Created by egordm on 24-8-2017.
//

#include <sstream>
#include <iostream>
#include "models.h"

namespace osupp {

    std::string BeatmapEntry::get_path(std::string osu_root) {
        std::ostringstream ret(osu_root);
        ret << "\\" << "Songs" << "\\" << folder_name << "\\" << osu_file;
        return ret.str();
    }

    bool Slider::in_slider(unsigned long t, float sliderMultiplayer, TimingPoint *tp) {
        return t >= time && t <= time + get_slider_duration(sliderMultiplayer, tp) * repeat;
    }

    Coordinate Slider::pos_at(unsigned long t, float sliderMultiplayer, TimingPoint *tp) {
        unsigned long duration = get_slider_duration(sliderMultiplayer, tp);
        if (duration == 0) return curve.position_at(0);
        t -= time;
        float at = (float) (t % duration) / duration;
        int r = ((int) floor(t / duration)) % 2;
        return curve.position_at(std::abs(r - at));
    }

    unsigned long Slider::get_slider_duration(float sliderMultiplayer, TimingPoint *tp) {
        double velocity = 100 * sliderMultiplayer / tp->get_mpb() * tp->slider_multiplayer;
        return static_cast<unsigned long>(ceil(pixel_length / velocity));
    }
}