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

class CurveBuilder {
public:
    explicit CurveBuilder(const std::vector<Coordinate> &ps) : points(ps) {};

    virtual CurveType build() { return CurveType(); };

protected:
    std::vector<Coordinate> points;
};

class LinearBuilder : public CurveBuilder {
public:
    LinearBuilder(const std::vector<Coordinate> &ps) : CurveBuilder(ps) {};

    virtual CurveType build() { return CurveType(points); };
};


// Credit to McKay for bezier approximator code
// https://github.com/McKay42/McOsu/blob/28246fd0bc897a8be4e519b51eea4fba10d49fcf/src/App/Osu/OsuSlider.cpp
class BezierBuilder : public CurveBuilder {
public:
    BezierBuilder(const std::vector<Coordinate> &ps);

    CurveType build() override;

    bool isFlatEnough(std::vector<Coordinate> controlPoints);

    void subdivide(std::vector<Coordinate> &controlPoints, std::vector<Coordinate> &l, std::vector<Coordinate> &r);

    void approximate(std::vector<Coordinate> &controlPoints, std::vector<Coordinate> &output);

private:
    static float TOLERANCE;
    static float TOLERANCE_SQ;
    unsigned int pointsCount;
    std::vector<Coordinate> subdivisionBuffer1;
    std::vector<Coordinate> subdivisionBuffer2;
};

class PerfectBuilder : public CurveBuilder {
public:
    explicit PerfectBuilder(const std::vector<Coordinate> &ps) : CurveBuilder(ps) {};

    CurveType build() override;

    bool isClose(float a, float b) { return std::abs(a-b)<0.01;};
};


#endif //OSUPP_CURVES_H
