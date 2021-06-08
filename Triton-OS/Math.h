#pragma once
#include <stdint.h>

namespace std {

    uint8_t abs(int8_t num);
    uint16_t abs(int16_t num);
    uint32_t abs(int32_t num);
    uint64_t abs(int64_t num);

    float log(float y);

    double pow(double base, double exp);
    double pow(double base, uint32_t exp);
    int64_t pow(int32_t base, uint32_t exp);

    float sqrt(float x);

    long double sqrt(long double x, uint16_t iterations);

    float Q_rsqrt(float number);

}