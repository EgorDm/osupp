//
// Created by egordm on 24-8-2017.
//

#include "maths.h"



Coordinate Coordinate::operator-() const {
    return Coordinate(-x, -y);
}

Coordinate Coordinate::operator+(const Coordinate& rhs) const {
    return Coordinate(x+rhs.x, y+rhs.y);
}

Coordinate Coordinate::operator-(const Coordinate& rhs) const {
    return Coordinate(x-rhs.x, y-rhs.y);
}

Coordinate& Coordinate::operator+=(const Coordinate& rhs) {
    x += rhs.x; y += rhs.y; return *this;
}

Coordinate& Coordinate::operator-=(const Coordinate& rhs) {
    x -= rhs.x; y -= rhs.y; return *this;
}

Coordinate Coordinate::operator*(const float a) const {
    return Coordinate(x*a, y*a);
}

Coordinate Coordinate::operator*(const Coordinate& rhs) const {
    return Coordinate(x*rhs.x, y*rhs.y);
}

Coordinate& Coordinate::operator*=(const float a) {
    x *= a; y *= a; return *this;
}

Coordinate& Coordinate::operator*=(const Coordinate& rhs) {
    x *= rhs.x; y *= rhs.y; return *this;
}

Coordinate Coordinate::operator/(const float a) const {
    return Coordinate(x/a, y/a);
}

Coordinate& Coordinate::operator/=(const float a) {
    x /= a; y /= a; return *this;
}

bool Coordinate::operator==(const Coordinate& rhs) const {
    return (x == rhs.x) && (y == rhs.y);
}

bool Coordinate::operator!=(const Coordinate& rhs) const {
    return (x != rhs.x) || (y != rhs.y);
}

bool Coordinate::operator<(const Coordinate& rhs) const {
    if(x < rhs.x) return true;
    if(x > rhs.x) return false;
    if(y < rhs.y) return true;
    if(y > rhs.y) return false;
    return false;
}

float Coordinate::operator[](int index) const {
    return (&x)[index];
}

float& Coordinate::operator[](int index) {
    return (&x)[index];
}

void Coordinate::zero() {
    this->x = 0.0f; this->y = 0.0f;
}

void Coordinate::set(float x, float y) {
    this->x = x; this->y = y;
}

float Coordinate::length() const {
    return std::sqrt(x*x + y*y);
}

float Coordinate::distance(const Coordinate& vec) const {
    return std::sqrt((vec.x-x)*(vec.x-x) + (vec.y-y)*(vec.y-y));
}

Coordinate& Coordinate::normalize() {
    float xxyy = x*x + y*y;
    if(xxyy < VECTOR_NORMALIZE_EPSILON)
        return *this; // do nothing if it is ~zero vector

    ///float invLength = invSqrt(xxyy);
    float invLength = 1.0f / std::sqrt(xxyy);
    x *= invLength;
    y *= invLength;
    return *this;
}

float Coordinate::dot(const Coordinate& rhs) const {
    return (x*rhs.x + y*rhs.y);
}

bool Coordinate::equal(const Coordinate& rhs, float epsilon) const {
    return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon;
}

Coordinate operator*(const float a, const Coordinate vec) {
    return Coordinate(a*vec.x, a*vec.y);
}

std::ostream& operator<<(std::ostream& os, const Coordinate& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
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
}