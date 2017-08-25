//
// Created by egordm on 24-8-2017.
//

#ifndef OSUPP_MATHS_H
#define OSUPP_MATHS_H

#include <cmath>
#include <vector>

class Coordinate {
public:
    float x, y;

    Coordinate() : x(0), y(0) {};

    Coordinate(float x, float y) : x(x), y(y) {}

    Coordinate &operator=(const Coordinate &c) {
        x = c.x;
        y = c.y;
        return *this;
    }

    Coordinate operator+(Coordinate &c) {
        return Coordinate(x + c.x, y + c.y);
    }

    Coordinate operator-(Coordinate &c) {
        return Coordinate(x - c.x, y - c.y);
    }

    Coordinate &operator+=(Coordinate &c) {
        x += c.x;
        y += c.y;
        return *this;
    }

    Coordinate &operator-=(Coordinate &c) {
        x -= c.x;
        y -= c.y;
        return *this;
    }

    Coordinate operator+(float s) {
        return Coordinate(x + s, y + s);
    }

    Coordinate operator-(float s) {
        return Coordinate(x - s, y - s);
    }

    Coordinate operator*(float s) {
        return Coordinate(x * s, y * s);
    }

    Coordinate operator/(float s) {
        return Coordinate(x / s, y / s);
    }

    Coordinate &operator+=(float s) {
        x += s;
        y += s;
        return *this;
    }

    Coordinate &operator-=(float s) {
        x -= s;
        y -= s;
        return *this;
    }

    Coordinate &operator*=(float s) {
        x *= s;
        y *= s;
        return *this;
    }

    Coordinate &operator/=(float s) {
        x /= s;
        y /= s;
        return *this;
    }

    float dist(Coordinate v) const {
        Coordinate d(v.x - x, v.y - y);
        return d.length();
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    static float dot(Coordinate v1, Coordinate v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }
};

class CurveBuilder {
public:
    explicit CurveBuilder(const std::vector<Coordinate> &ps) : points(ps) {};

    virtual std::vector<Coordinate> build() { return points; };

private:
    std::vector<Coordinate> points;
};

class BezierBuilder : public CurveBuilder {
public:
    BezierBuilder(const std::vector<Coordinate> &ps);

    std::vector<Coordinate> build() override;

    bool isFlatEnough(std::vector<Coordinate> controlPoints);

};

#endif //OSUPP_MATHS_H
