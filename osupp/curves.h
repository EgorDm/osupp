//
// Created by egordm on 24-8-2017.
//

#ifndef OSUPP_CURVES_H
#define OSUPP_CURVES_H

#include <vector>
#include "maths.h"

namespace osupp {

    class Curve {
    public:
        enum CurveType {
            None = 'N',
            Linear = 'L',
            Bezier = 'B',
            Perfect = 'P',
            Catmull = 'C'
        };

        explicit Curve(CurveType type, const std::vector<Coordinate> &points, double length);

        Curve(const std::vector<Coordinate> &points, double length);

        Coordinate positionAt(float progress);

        double length();

        inline CurveType getType() { return type; }

        inline std::vector<Coordinate> getPoints() { return points; }

        inline void setPoints(std::vector<Coordinate> &points) { this->points = points; }

    private:
        void calcCumLength();

        int indexOfDistance(double d);

        double progressToDistance(float progress);

        Coordinate interpolateVertices(int i, double d);

        CurveType type = None;
        std::vector<Coordinate> points;
        std::vector<Coordinate> original_points;
        std::vector<double> cum_length;
        double px_length = 0;
    };
}
#endif //OSUPP_CURVES_H
