#include "Math.h"

uint8_t std::abs(int8_t num) {
    return num >= 0 ? (uint8_t)num : (uint8_t)num * -1; 
}

uint16_t std::abs(int16_t num) {
    return num >= 0 ? (uint16_t)num : (uint16_t)num * -1; 
}

uint32_t std::abs(int32_t num) {
    return num >= 0 ? (uint32_t)num : (uint32_t)num * -1; 
}

uint64_t std::abs(int64_t num) {
    return num >= 0 ? (uint64_t)num : (uint64_t)num * -1; 
}

#define EXP_A 184
#define EXP_C 16249

float std::log(float y) {

    int* nTemp = (int*)&y;
    y = (*nTemp) >> 16;
    return (y - EXP_C) / EXP_A;
}

double std::pow(double base, double exp) {

    union {
        double d;
        int x[2];
    } u = { base };

    u.x[1] = (int)(exp * (u.x[1] - 1072632447) + 1072632447);
    u.x[0] = 0;

    return u.d;
}

double std::pow(double base, uint32_t exp) {

    double ret = base;
    for (uint32_t i = 1; i < exp; i++) {
        ret *= base;
    }

    return ret;
}

int64_t std::pow(int32_t base, uint32_t exp) {

    uint64_t ret = base;
    for (uint32_t i = 1; i < exp; i++) {
        ret *= base;
    }

    return ret;
}

float std::sqrt(float x) {

    // Newton's method
    float w = 0.5 * (1 + x);
    for (int i = 0; i <= 8; ++i) {
        double y = w * w - x;
        double m = 2 * w;
        w -= y / m;
    }

    return w;
}

long double std::sqrt(long double x, uint16_t iterations) {
    // Newton's method
    if (iterations <= 50) iterations = 50;
    double w = 0.5 * (1 + x);
    for (int i = 0; i <= iterations; ++i) {
        double y = w * w - x;
        double m = 2 * w;
        w -= y / m;
    }

    return w;
}

float std::Q_rsqrt(float number) {

    union {
        float f;
        uint32_t i;
    } conv = { .f = number };

    conv.i = 0x5F1FFFF9 - (conv.i >> 1);
    conv.f *= 0.703952253f * (2.38924456f - number * conv.f * conv.f);

    return conv.f;
}