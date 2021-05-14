#pragma once

#include <stddef.h>

struct Framebuffer {
    void* BaseAddress;
    size_t BufferSize;
    unsigned int Width;
    unsigned int Heigth;
    unsigned int PixelsPerScanLine;
};