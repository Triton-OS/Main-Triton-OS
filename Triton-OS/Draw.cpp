#include "Draw.h"

/*void Draw::Init(Framebuffer* TargetFramebuffer) { Draw::TargetFramebuffer = TargetFramebuffer; return; }*/

void Draw::Rectangle(uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t heigth, uint32_t color) {

	while (pos_x <= width) {
		for(uint16_t y = pos_y; y <= width; y++)
			*(uint32_t*)((uint64_t)this->TargetFramebuffer->BaseAddress + (pos_x * 4) + (y * this->TargetFramebuffer->PixelsPerScanLine * 4)) = color;  // = setPixel

		pos_x++;
	}

	return;
}

void Draw::Circle(uint16_t midpoint_x, uint16_t midpoint_y, uint16_t radius, uint32_t color) {

    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    *(uint32_t*)((uint64_t)this->TargetFramebuffer->BaseAddress + (midpoint_x * 4) + ((midpoint_y + radius) * this->TargetFramebuffer->PixelsPerScanLine * 4)) = color;  // = setPixel
    *(uint32_t*)((uint64_t)this->TargetFramebuffer->BaseAddress + (midpoint_x * 4) + ((midpoint_y - radius) * this->TargetFramebuffer->PixelsPerScanLine * 4)) = color;  // = setPixel
    *(uint32_t*)((uint64_t)this->TargetFramebuffer->BaseAddress + ((midpoint_x + radius) * 4) + (midpoint_y * this->TargetFramebuffer->PixelsPerScanLine * 4)) = color;  // = setPixel
    *(uint32_t*)((uint64_t)this->TargetFramebuffer->BaseAddress + ((midpoint_x - radius) * 4) + (midpoint_y * this->TargetFramebuffer->PixelsPerScanLine * 4)) = color;  // = setPixel

    while (x < y) {

        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }

        x++;
        ddF_x += 2;
        f += ddF_x + 1;

        *(uint32_t*)((uint64_t)this->TargetFramebuffer->BaseAddress + ((midpoint_x + x)* 4) + ((midpoint_y + y) * this->TargetFramebuffer->PixelsPerScanLine * 4)) = color;  // = setPixel
        *(uint32_t*)((uint64_t)this->TargetFramebuffer->BaseAddress + ((midpoint_x - x)* 4) + ((midpoint_y + y) * this->TargetFramebuffer->PixelsPerScanLine * 4)) = color;  // = setPixel
        *(uint32_t*)((uint64_t)this->TargetFramebuffer->BaseAddress + ((midpoint_x + x)* 4) + ((midpoint_y - y) * this->TargetFramebuffer->PixelsPerScanLine * 4)) = color;  // = setPixel
        *(uint32_t*)((uint64_t)this->TargetFramebuffer->BaseAddress + ((midpoint_x - x)* 4) + ((midpoint_y - y) * this->TargetFramebuffer->PixelsPerScanLine * 4)) = color;  // = setPixel
        
        *(uint32_t*)((uint64_t)this->TargetFramebuffer->BaseAddress + ((midpoint_x + y) * 4) + ((midpoint_y + x) * this->TargetFramebuffer->PixelsPerScanLine * 4)) = color;  // = setPixel
        *(uint32_t*)((uint64_t)this->TargetFramebuffer->BaseAddress + ((midpoint_x - y) * 4) + ((midpoint_y + x) * this->TargetFramebuffer->PixelsPerScanLine * 4)) = color;  // = setPixel
        *(uint32_t*)((uint64_t)this->TargetFramebuffer->BaseAddress + ((midpoint_x + y) * 4) + ((midpoint_y - x) * this->TargetFramebuffer->PixelsPerScanLine * 4)) = color;  // = setPixel
        *(uint32_t*)((uint64_t)this->TargetFramebuffer->BaseAddress + ((midpoint_x - y) * 4) + ((midpoint_y - x) * this->TargetFramebuffer->PixelsPerScanLine * 4)) = color;  // = setPixel
    }

    return;
}

void Draw::Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color) {

    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    while (true) {
        *(uint32_t*)((uint64_t)this->TargetFramebuffer->BaseAddress + (x0 * 4) + (y0 * this->TargetFramebuffer->PixelsPerScanLine * 4)) = color;  // = setPixel

        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;

        if (e2 > dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if (e2 < dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
    }

    return;
}
