//
// Created by egordm on 24-8-2017.
//
#define _USE_MATH_DEFINES

#include <stack>
#include <algorithm>
#include <cmath>
#include "maths.h"
#include "utils.h"

Coordinate Coordinate::operator-() const {
    return Coordinate(-x, -y);
}

Coordinate Coordinate::operator+(const Coordinate &rhs) const {
    return Coordinate(x + rhs.x, y + rhs.y);
}

Coordinate Coordinate::operator-(const Coordinate &rhs) const {
    return Coordinate(x - rhs.x, y - rhs.y);
}

Coordinate &Coordinate::operator+=(const Coordinate &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Coordinate &Coordinate::operator-=(const Coordinate &rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Coordinate Coordinate::operator*(const float a) const {
    return Coordinate(x * a, y * a);
}

Coordinate Coordinate::operator*(const Coordinate &rhs) const {
    return Coordinate(x * rhs.x, y * rhs.y);
}

Coordinate &Coordinate::operator*=(const float a) {
    x *= a;
    y *= a;
    return *this;
}

Coordinate &Coordinate::operator*=(const Coordinate &rhs) {
    x *= rhs.x;
    y *= rhs.y;
    return *this;
}

Coordinate Coordinate::operator/(const float a) const {
    return Coordinate(x / a, y / a);
}

Coordinate &Coordinate::operator/=(const float a) {
    x /= a;
    y /= a;
    return *this;
}

bool Coordinate::operator==(const Coordinate &rhs) const {
    return (x == rhs.x) && (y == rhs.y);
}

bool Coordinate::operator!=(const Coordinate &rhs) const {
    return (x != rhs.x) || (y != rhs.y);
}

bool Coordinate::operator<(const Coordinate &rhs) const {
    if (x < rhs.x) return true;
    if (x > rhs.x) return false;
    if (y < rhs.y) return true;
    if (y > rhs.y) return false;
    return false;
}

float Coordinate::operator[](int index) const {
    return (&x)[index];
}

float &Coordinate::operator[](int index) {
    return (&x)[index];
}

void Coordinate::zero() {
    this->x = 0.0f;
    this->y = 0.0f;
}

void Coordinate::set(float x, float y) {
    this->x = x;
    this->y = y;
}

float Coordinate::length() const {
    return std::sqrt(x * x + y * y);
}

float Coordinate::distance(const Coordinate &vec) const {
    return std::sqrt((vec.x - x) * (vec.x - x) + (vec.y - y) * (vec.y - y));
}

Coordinate &Coordinate::normalize() {
    float xxyy = x * x + y * y;
    if (xxyy < VECTOR_NORMALIZE_EPSILON)
        return *this; // do nothing if it is ~zero vector

    ///float invLength = invSqrt(xxyy);
    float invLength = 1.0f / std::sqrt(xxyy);
    x *= invLength;
    y *= invLength;
    return *this;
}

float Coordinate::dot(const Coordinate &rhs) const {
    return (x * rhs.x + y * rhs.y);
}

bool Coordinate::equal(const Coordinate &rhs, float epsilon) const {
    return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon;
}

Coordinate operator*(const float a, const Coordinate vec) {
    return Coordinate(a * vec.x, a * vec.y);
}

std::ostream &operator<<(std::ostream &os, const Coordinate &vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

float *Coordinate::toArray() {
    return new float[2]{x, y};
}

Coordinate &Coordinate::normalize(const float xmax, const float ymax) {
    x /= xmax;
    y /= ymax;
    return *this;
}

Coordinate &Coordinate::clamp(const float xmin, const float xmax, const float ymin, const float ymax) {
    x = utils::clamp(x, xmin, xmax);
    y = utils::clamp(y, ymin, ymax);
    return *this;
}

namespace maths {
    const float TOLERANCE_SQ = 0.25f * 0.25f;

    const float catmullAlpha = 0.5f;

    bool isClose(float a, float b) { return std::abs(a - b) < 0.01; };

    void subdivide(Coords &controlPoints, Coords &l, Coords &r, unsigned long pointsCount, Coords &subdivisionBuffer1) {
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

    bool isFlatEnough(Coords controlPoints) {
        for (int i = 1; i < controlPoints.size() - 1; i++) {

            if (std::pow((controlPoints[i - 1] - 2 * controlPoints[i] + controlPoints[i + 1]).length(), 2.0f) >
                TOLERANCE_SQ * 4)
                return false;
        }

        return true;
    }

    void approximate(Coords &controlPoints, Coords &output, unsigned long pointsCount, Coords &subdivisionBuffer1,
                     Coords &subdivisionBuffer2) {
        std::vector<Coordinate> &l = subdivisionBuffer2;
        std::vector<Coordinate> &r = subdivisionBuffer1;

        subdivide(controlPoints, l, r, pointsCount, subdivisionBuffer1);

        for (int i = 0; i < pointsCount - 1; ++i)
            l[pointsCount + i] = r[i + 1];

        output.push_back(controlPoints[0]);
        for (int i = 1; i < pointsCount - 1; ++i) {
            int index = 2 * i;
            Coordinate p = 0.25f * (l[index - 1] + 2 * l[index] + l[index + 1]);
            output.push_back(p);
        }
    }

    Coords bezierCurve(Coords points) {
        std::vector<Coordinate> output;
        unsigned long pointsCount = points.size();
        Coords subdivisionBuffer1(pointsCount);
        Coords subdivisionBuffer2(pointsCount * 2 - 1);

        if (pointsCount == 0)
            return output;

        std::stack<std::vector<Coordinate>> toFlatten;
        std::stack<std::vector<Coordinate>> freeBuffers;

        toFlatten.push(points);

        std::vector<Coordinate> &leftChild = subdivisionBuffer2;

        while (!toFlatten.empty()) {
            std::vector<Coordinate> parent = static_cast<std::vector<Coordinate> &&>(toFlatten.top());
            toFlatten.pop();

            if (isFlatEnough(parent)) {
                approximate(parent, output, pointsCount, subdivisionBuffer1, subdivisionBuffer2);
                freeBuffers.push(parent);
                continue;
            }

            std::vector<Coordinate> rightChild;
            if (!freeBuffers.empty()) {
                rightChild = freeBuffers.top();
                freeBuffers.pop();
            } else {
                rightChild.resize(pointsCount);
            }
            subdivide(parent, leftChild, rightChild, pointsCount, subdivisionBuffer1);

            for (int i = 0; i < pointsCount; ++i) {
                parent[i] = leftChild[i];
            }

            toFlatten.push(rightChild);
            toFlatten.push(parent);
        }

        output.push_back(points[pointsCount - 1]);
        return output;
    }

    std::vector<Coordinate> perfectCurve(Coordinate a, Coordinate b, Coordinate c) {
        // TODO this is a mess
        float aSq = std::pow((b - c).length(), 2);
        float bSq = std::pow((a - c).length(), 2);
        float cSq = std::pow((a - b).length(), 2);

        if (isClose(aSq, 0) || isClose(bSq, 0) || isClose(cSq, 0))
            return {a};

        float s = aSq * (bSq + cSq - aSq);
        float t = bSq * (aSq + cSq - bSq);
        float u = cSq * (aSq + bSq - cSq);

        float sum = s + t + u;

        if (isClose(sum, 0))
            return {a};

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

        return output;
    }

    Coordinate catmullPoint(Coordinate p1, Coordinate p2, Coordinate p3, Coordinate p4, float t) {
        return catmullAlpha * ((-p1 + 3 * p2 - 3 * p3 + p4) * std::pow(t, 3)
                               + (2 * p1 - 5 * p2 + 4 * p3 - p4) * std::pow(t, 2)
                               + (-p1 + p3) * t + 2 * p2); // TODO: Calculate and reuse
    }

    std::vector<Coordinate> catmullCurve(Coordinate p1, Coordinate p2, Coordinate p3, Coordinate p4, float interval) {
        std::vector<Coordinate> ret;
        for (float t = 0; t <= 1.0; t += interval) {
            ret.push_back(catmullPoint(p1, p2, p3, p4, t));
        }
        return ret;
    }

    std::vector<Coordinate> catmullChain(Coords points, float interval) {
        std::vector<Coordinate> ret;
        for (int i = 0; i < points.size() - 3; i++) {
            std::vector<Coordinate> ps = catmullCurve(points[i], points[i + 1], points[i + 2], points[i + 3], interval);
            ret.insert(ret.end(), ps.begin(), ps.end());
        }
        return ret;
    }

    double clamp(double x, double lower, double upper) {
        return std::min(upper, std::max(x, lower));
    }

}