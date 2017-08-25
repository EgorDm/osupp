//
// Created by egordm on 24-8-2017.
//

#include "maths.h"


BezierBuilder::BezierBuilder(const std::vector<Coordinate> &ps) : CurveBuilder(ps) {}

std::vector<Coordinate> BezierBuilder::build() {
    return CurveBuilder::build();
}

bool BezierBuilder::isFlatEnough(std::vector<Coordinate> controlPoints) {
/*    for (int i = 1; i < controlPoints.size() - 1; i++) {
        if (std::pow((controlPoints[i - 1] - 2 * controlPoints[i] + controlPoints[i + 1]).length(), 2.0f) >
            TOLERANCE_SQ * 4)
            return false;
    }*/

    return true;
}
