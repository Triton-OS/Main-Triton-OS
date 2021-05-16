#pragma once

#include "Math.h"
#include "Framebuffer.h"
#include "BasicFont.h"
#include <stdint.h>


#define STD_BASIC_RENDERER_GLYPH_HIGTH 18
#define STD_BASIC_RENDERER_GLYPH_WIDTH 8

class BasicRenderer
{

public:

	Framebuffer* TargetFramebuffer;
	PSF1_FONT* psf1_font;
	uint32_t color, back_color;
	uint32_t MouseCursorBuffer[16 * 16];

	BasicRenderer(Framebuffer* TargetFramebuffer, PSF1_FONT* psf1_font, uint32_t color = 0xffffffff, uint32_t back_color = 0x00000000);

	void SetColor(unsigned int color);

	void PutPixel(Point pos);
	void PutPixel(uint32_t x, uint32_t y);
	void PutPixel(Point pos, uint32_t color);
	void PutPixel(uint32_t x, uint32_t y, uint32_t color);

	uint32_t GetPixel(Point pos);
	uint32_t GetPixel(uint32_t x, uint32_t y);

	void DrawOverlayMouseCursor(uint8_t* mouseCursor, Point pos);
	void DrawOverlayMouseCursor(uint8_t* mouseCursor, Point pos, uint32_t color);

	void ClearMouseCursor(uint8_t* mouseCursor, Point pos);

	void Print(char chr);
	void Print(const char* str);
	void Print(const char* str, uint32_t color);

	void PutChar(char c);
	void PutChar(char c, uint32_t color);
	void PutChar(char c, uint32_t xOff, uint32_t yOff, uint32_t color);

	void Endl();

	void ClearLastChar();
	void ClearLastChar(uint32_t clear_color);

	void Clear();
	void Clear(uint32_t color = 0x00000000);



	Point CursorPosition = { 0, 0 };

};

extern BasicRenderer* GlobalRenderer;