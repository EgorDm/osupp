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
}