//
// Created by egordm on 24-8-2017.
//

#ifndef OSUPP_CURVES_H
#define OSUPP_CURVES_H

#include <vector>
#include "maths.h"

class Curve {
public:
    enum CurveType {
        Linear = 'L',
        Bezier = 'B',
        Perfect = 'P',
        Catmull = 'C'
    };

    explicit Curve(CurveType type, const std::vector<Coordinate> &points, double length);

    Curve() {};

    Coordinate positionAt(double progress);

    double length();

    inline CurveType getType() { return type; }

private:
    void calcCumLength();

    int indexOfDistance(double d);

    double progressToDistance(double progress);

    Coordinate interpolateVertices(int i, double d);

    CurveType type;
    std::vector<Coordinate> points;
    std::vector<double> cumLength;
    double lenOrigin = 0;

};

#endif //OSUPP_CURVES_H
