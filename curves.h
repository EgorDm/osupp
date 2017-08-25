//
// Created by egordm on 24-8-2017.
//

#ifndef OSUPP_CURVES_H
#define OSUPP_CURVES_H

#include <vector>
#include "maths.h"

class CurveType {
public:
    CurveType(const std::vector<Coordinate> &points);

    CurveType() {};

    std::vector<Coordinate> points;
    std::vector<float> cum_length;
    float distance;
};

#endif //OSUPP_CURVES_H
