//
// Created by egordm on 24-8-2017.
//

#include <stack>
#include <iostream>
#include <algorithm>
#include "curves.h"

using namespace maths;

Curve::Curve(const std::vector<Coordinate> &points, double length) : points(points), lenOrigin(length) {

}


Curve::Curve(CurveType type, const std::vector<Coordinate> &points, double length) : type(type), lenOrigin(length) {
    switch (type) {
        case Linear:
            this->points = points;
            break;
        case Bezier:
            this->points = maths::bezierCurve(points);
            break;
        case Perfect:
            if(points.size() < 3) throw std::out_of_range("Perfect curve must at least have 3 points!");
            this->points = maths::perfectCurve(points[0], points[1], points[2]);
            break;
        case Catmull:
            if(points.size() >= 4) this->points = maths::catmullChain(points, 0.1);
            else this->points = maths::bezierCurve(points);
            break;
    }
}

void Curve::calcCumLength() {

    double l = 0;
    cumLength.clear();
    cumLength.push_back(l);

    // TODO: calc path
    for (int i = 0; i < points.size() - 1; ++i) {
        Coordinate diff = points[i + 1] - points[i];
        double d = diff.length();

        if (lenOrigin - l < d) {
            //points[i + 1] = points[i] + diff * (float) ((lenOrigin - l) / d);
            // points.erase(points.begin() + i + 2, points.begin() + points.size() - 2 - i); //TODO use points.end()

            l = lenOrigin;
            cumLength.push_back(l);
            break;
        }

        l += d;
        cumLength.push_back(l);
    }
}

int Curve::indexOfDistance(double d) {
    int i = static_cast<int>(maths::binary_locate(cumLength.begin(), cumLength.end(), d) - cumLength.begin());
    if (i < 0) i = ~i;
    return i;
}

double Curve::progressToDistance(float progress) {
    return maths::clamp(progress, 0, 1) * lenOrigin;
}

Coordinate Curve::interpolateVertices(int i, double d) {
    if (points.size() == 0)
        return Coordinate();

    if (i <= 0)
        return points.front();
    else if (i >= points.size())
        return points.back();

    Coordinate p0 = points[i - 1];
    Coordinate p1 = points[i];

    double d0 = cumLength[i - 1];
    double d1 = cumLength[i];

    if (maths::isClose(d0, d1))
        return p0;

    double w = (d - d0) / (d1 - d0);
    return p0 + (p1 - p0) * (float) w;
}

Coordinate Curve::positionAt(float progress) {
    if (cumLength.empty()) calcCumLength();

    double d = progressToDistance(progress);
    return interpolateVertices(indexOfDistance(d), d);
}

double Curve::length() {
    double l = 0;
    for (int i = 1; i < points.size(); ++i) {
        l += (points[i] - points[i - 1]).length();
    }
    return l;
}

