#pragma once

#include "Math.h"
#include "Framebuffer.h"
#include "BasicFont.h"
#include <stdint.h>

enum Zoom { Normal = 1, X1 = 1, X2 = 2, X3 = 3, X4 = 4, X5 = 5, X6 = 6};

class BasicRenderer
{
public:
	BasicRenderer(Framebuffer* TargetFramebuffer, PSF1_FONT* psf1_font, uint32_t color = 0xffffffff, Zoom zoom = Zoom::Normal);

	Framebuffer* TargetFramebuffer;
	PSF1_FONT* psf1_font;
	Zoom zoom;
	unsigned int color;

	void SetColor(unsigned int color);
	void SetZoom(Zoom z = Zoom::Normal);

	void Print(char chr, Zoom z = Zoom::Normal);
	void Print(const char* str, Zoom z = Zoom::Normal);
	void Print(const char* str, unsigned int colour, Zoom z = Zoom::Normal);
	void PutChar(char c, unsigned int xOff, unsigned int yOff, unsigned int colour, Zoom z = Zoom::Normal);

	void Clear();
	void Clear(uint32_t color = 0x00000000);



	Point CursorPosition = {0, 0};

};

extern BasicRenderer* GlobalRenderer;