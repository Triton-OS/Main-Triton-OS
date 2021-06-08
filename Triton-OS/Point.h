#pragma once
#include <stdint.h>

struct Point32 {
    Point32(int32_t X, int32_t Y) { this->X = X; this->Y = Y; }
    int32_t X;
    int32_t Y;
};

struct uPoint32 {
    uPoint32(uint32_t X, uint32_t Y) { this->X = X; this->Y = Y; }
    uint32_t X;
    uint32_t Y;
};

struct Point {
    Point(int64_t X, int64_t Y) { this->X = X; this->Y = Y; }
    int64_t X;
    int64_t Y;
};

struct uPoint {
    uPoint(uint64_t X, uint64_t Y) { this->X = X; this->Y = Y; }
    uint64_t X;
    uint64_t Y;
};