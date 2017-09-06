//
// Created by egordm on 24-8-2017.
//

#include <sstream>
#include "models.h"

std::string BeatmapEntry::getPath(std::string osu_root) {
    std::ostringstream ret(osu_root);
    ret << "\\" << "Songs" << "\\" << folder_name << "\\" << osu_file;
    return ret.str();
}

bool Slider::inSlider(unsigned long t, float sliderMultiplayer, TimingPoint *tp) {
    return t > time && t < time + getSliderDuration(sliderMultiplayer, tp) * repeat;
}

Coordinate Slider::posAt(unsigned long t, float sliderMultiplayer, TimingPoint *tp) {
    unsigned long duration = getSliderDuration(sliderMultiplayer, tp);
    if(duration == 0) return curve.positionAt(0);
    t -= time;
    float at = (t % duration) / duration;
    int r = ((int)floor(t / duration)) % 2;
    return curve.positionAt(std::abs(r - at));
}

unsigned long Slider::getSliderDuration(float sliderMultiplayer, TimingPoint *tp) {
    double velocity = 100 * sliderMultiplayer / tp->sliderMultiplayer / tp->getMPB();
    return static_cast<unsigned long>(pixelLength / velocity);
}
