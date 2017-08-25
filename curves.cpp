//
// Created by egordm on 24-8-2017.
//

#include <stack>
#include <iostream>
#include "curves.h"

float BezierBuilder::TOLERANCE = 0.25f;
float BezierBuilder::TOLERANCE_SQ = 0.25f * 0.25f;

CurveType::CurveType(const std::vector<Coordinate> &points) : points(points) {

}

BezierBuilder::BezierBuilder(const std::vector<Coordinate> &ps) : CurveBuilder(ps) {
    pointsCount = ps.size();
    subdivisionBuffer1.resize(pointsCount);
    subdivisionBuffer2.resize(pointsCount * 2 - 1);
}

CurveType BezierBuilder::build() {
    std::vector<Coordinate> output;
    if (pointsCount == 0)
        return CurveType(output);

    std::stack<std::vector<Coordinate>> toFlatten;
    std::stack<std::vector<Coordinate>> freeBuffers;

    toFlatten.push(points);

    std::vector<Coordinate> &leftChild = subdivisionBuffer2;

    while (!toFlatten.empty()) {
        std::vector<Coordinate> parent = static_cast<std::vector<Coordinate> &&>(toFlatten.top());
        toFlatten.pop();

        if (isFlatEnough(parent)) {
            approximate(parent, output);
            freeBuffers.push(parent);
            continue;
        }

        std::vector<Coordinate> rightChild;
        if (!freeBuffers.empty()) {
            rightChild = freeBuffers.top();
            freeBuffers.pop();
        } else
            rightChild.resize(pointsCount);
        subdivide(parent, leftChild, rightChild);

        for (int i = 0; i < pointsCount; ++i) {
            parent[i] = leftChild[i];
        }

        toFlatten.push(rightChild);
        toFlatten.push(parent);
    }

    output.push_back(points[pointsCount - 1]);
    return CurveType(output);
}

bool BezierBuilder::isFlatEnough(std::vector<Coordinate> controlPoints) {
    for (int i = 1; i < controlPoints.size() - 1; i++) {

        if (std::pow((controlPoints[i - 1] - 2 * controlPoints[i] + controlPoints[i + 1]).length(), 2.0f) >
            TOLERANCE_SQ * 4)
            return false;
    }

    return true;
}

void BezierBuilder::subdivide(std::vector<Coordinate> &controlPoints, std::vector<Coordinate> &l,
                              std::vector<Coordinate> &r) {
    std::vector<Coordinate> &midpoints = subdivisionBuffer1;

    for (int i = 0; i < pointsCount; ++i) {
        midpoints[i] = controlPoints[i];
    }

    for (int i = 0; i < pointsCount; i++) {
        l[i] = midpoints[0];
        r[pointsCount - i - 1] = midpoints[pointsCount - i - 1];

        for (int j = 0; j < pointsCount - i - 1; j++) {
            midpoints[j] = (midpoints[j] + midpoints[j + 1]) / 2;
        }
    }
}

void BezierBuilder::approximate(std::vector<Coordinate> &controlPoints, std::vector<Coordinate> &output) {
    std::vector<Coordinate> &l = subdivisionBuffer2;
    std::vector<Coordinate> &r = subdivisionBuffer1;

    subdivide(controlPoints, l, r);

    for (int i = 0; i < pointsCount - 1; ++i)
        l[pointsCount + i] = r[i + 1];

    output.push_back(controlPoints[0]);
    for (int i = 1; i < pointsCount - 1; ++i) {
        int index = 2 * i;
        Coordinate p = 0.25f * (l[index - 1] + 2 * l[index] + l[index + 1]);
        output.push_back(p);
    }
}

CurveType PerfectBuilder::build() {
    // TODO this is a mess
    Coordinate a = points[0];
    Coordinate b = points[1];
    Coordinate c = points[2];

    float aSq = std::pow((b - c).length(), 2);
    float bSq = std::pow((a - c).length(), 2);
    float cSq = std::pow((a - b).length(), 2);

    if (isClose(aSq, 0) || isClose(bSq, 0) || isClose(cSq, 0)) {
        std::vector<Coordinate> ret;
        ret.push_back(points[0]);
        return CurveType(ret);
    }

    float s = aSq * (bSq + cSq - aSq);
    float t = bSq * (aSq + cSq - bSq);
    float u = cSq * (aSq + bSq - cSq);

    float sum = s + t + u;

    if (isClose(sum, 0)) {
        std::vector<Coordinate> ret;
        ret.push_back(points[0]);
        return CurveType(ret);
    }

    Coordinate centre = (s * a + t * b + u * c) / sum;
    Coordinate dA = a - centre;
    Coordinate dC = c - centre;
    float r = dA.length();

    float thetaStart = atan2(dA.y, dA.x);
    float thetaEnd = atan2(dC.y, dC.x);

    while (thetaEnd < thetaStart)
        thetaEnd += 2 * M_PI;

    int dir = 1;
    float thetaRange = thetaEnd - thetaStart;

    Coordinate orthoAtoC = c - a;
    orthoAtoC = Coordinate(orthoAtoC.y, -orthoAtoC.x);
    if (orthoAtoC.dot(b - a) < 0) {
        dir = -dir;
        thetaRange = 2 * M_PI - thetaRange;
    }

    int amountPoints;
    if (2 * r <= 0.1) {
        amountPoints = 2;
    } else {
        amountPoints = std::max(2, static_cast<const int &>(ceil(thetaRange / (2 * acos(1 - 0.1 / r)))));
    }

    std::vector<Coordinate> output;
    for (int i = 0; i < amountPoints; ++i) {
        float fract = static_cast<float>(i) / (amountPoints - 1);
        float theta = thetaStart + dir * fract * thetaRange;
        Coordinate o = Coordinate(cos(theta), sin(theta)) * r;
        output.push_back(centre + o);
    }

    return CurveType(output);
}

