#include "BasicRenderer.h"

BasicRenderer* GlobalRenderer;

BasicRenderer::BasicRenderer(Framebuffer* TargetFramebuffer, PSF1_FONT* psf1_font, uint32_t color, uint32_t back_color) {
	this->TargetFramebuffer = TargetFramebuffer;
	this->CursorPosition = { 0, 0 };
	this->psf1_font = psf1_font;
	this->color = color;
	this->back_color = back_color;
}

void BasicRenderer::SetColor(uint32_t color) {
	this->color = color;
	return;
}

void BasicRenderer::PutPixel(Point pos) {
	this->PutPixel(pos.X, pos.Y, this->color);
	return;
}

void BasicRenderer::PutPixel(uint32_t x, uint32_t y) {
	this->PutPixel(x, y, this->color);
	return;
}

void BasicRenderer::PutPixel(Point pos, uint32_t color) {
	this->PutPixel(pos.X, pos.Y, color);
	return;
}

void BasicRenderer::PutPixel(uint32_t x, uint32_t y, uint32_t color) {
	*(uint32_t*)((uint64_t)this->TargetFramebuffer->BaseAddress + (x * 4) + (y * this->TargetFramebuffer->PixelsPerScanLine * 4)) = color;
	return;
}

uint32_t BasicRenderer::GetPixel(Point pos) {
	return this->GetPixel(pos.X, pos.Y);
}

uint32_t BasicRenderer::GetPixel(uint32_t x, uint32_t y) {
	return *(uint32_t*)((uint64_t)this->TargetFramebuffer->BaseAddress + (x * 4) + (y * this->TargetFramebuffer->PixelsPerScanLine * 4));
}

void BasicRenderer::DrawOverlayMouseCursor(uint8_t* mouseCursor, Point pos) {
	this->DrawOverlayMouseCursor(mouseCursor, pos, this->color);
	return;
}

void BasicRenderer::DrawOverlayMouseCursor(uint8_t* mouseCursor, Point pos, uint32_t color) {
	uint8_t xMax = 16;
	uint8_t yMax = 21;
	int differenceX = this->TargetFramebuffer->Width - pos.X;
	int differenceY = this->TargetFramebuffer->Heigth - pos.Y;

	if (differenceX < 16) xMax = differenceX;
	if (differenceY < 21) yMax = differenceY;

	for (int y = 0; y < yMax; y++) {
		for (int x = 0; x < xMax; x++) {
			int bit = y * 16 + x;
			int byte = bit / 8;
			if (mouseCursor[byte] & (0b10000000 >> (x % 8))) {
				MouseCursorBuffer[x + y * 16] = this->GetPixel(pos.X + x, pos.Y + y);
				this->PutPixel(pos.X + x, pos.Y + y, color);
			}
		}
	}

	return;
}

void BasicRenderer::ClearMouseCursor(uint8_t* mouseCursor, Point pos) {
	uint8_t xMax = 16;
	uint8_t yMax = 21;
	int differenceX = this->TargetFramebuffer->Width - pos.X;
	int differenceY = this->TargetFramebuffer->Heigth - pos.Y;

	if (differenceX < 16) xMax = differenceX;
	if (differenceY < 21) yMax = differenceY;

	for (int y = 0; y < yMax; y++) {
		for (int x = 0; x < xMax; x++) {
			int bit = y * 16 + x;
			int byte = bit / 8;
			if (mouseCursor[byte] & (0b10000000 >> (x % 8))) {
				this->PutPixel(pos.X + x, pos.Y + y, MouseCursorBuffer[x + y * 16]);
			}
		}
	}

	return;
}

void BasicRenderer::Print(char chr) {
		PutChar(chr, this->color);
}

void BasicRenderer::Print(const char* str) {
	this->Print(str, this->color);
	return;
}

void BasicRenderer::Print(const char* str, uint32_t color) {
	char* c = (char*)str;

	while (*c != '\0') {
		PutChar(*c, color);
		c++;
	}

	return;
}

void BasicRenderer::PutChar(char c) {
	this->PutChar(c, this->color);
	return;
}

void BasicRenderer::PutChar(char c, uint32_t color) {
	if (c == '\r') this->CursorPosition.X = 0; 
	else if (c == '\n') {
		if (this->CursorPosition.Y + STD_BASIC_RENDERER_GLYPH_HIGTH < this->TargetFramebuffer->Heigth)
			this->CursorPosition.Y += STD_BASIC_RENDERER_GLYPH_HIGTH; 
	}
	else {
		if (this->CursorPosition.X + STD_BASIC_RENDERER_GLYPH_WIDTH >= this->TargetFramebuffer->Width) {
			this->CursorPosition.Y += STD_BASIC_RENDERER_GLYPH_HIGTH;
			if (this->CursorPosition.Y >= this->TargetFramebuffer->Heigth) this->CursorPosition.Y = this->TargetFramebuffer->Heigth - STD_BASIC_RENDERER_GLYPH_HIGTH;
			else this->CursorPosition.X = 0;
		}
		this->PutChar(c, this->CursorPosition.X, this->CursorPosition.Y, color);
		this->CursorPosition.X += STD_BASIC_RENDERER_GLYPH_WIDTH;
	}
	return;
}

void BasicRenderer::PutChar(char c, uint32_t xOff, uint32_t yOff, uint32_t color) {

	unsigned int* pixPtr = (unsigned int*)this->TargetFramebuffer->BaseAddress;
	char* fontPtr = (char*)this->psf1_font->glyphBuffer + (c * this->psf1_font->psf1_header->charsize);
	
	for (unsigned long y = yOff; y < yOff + STD_BASIC_RENDERER_GLYPH_HIGTH; y++) {
		for (unsigned long x = xOff; x < xOff + STD_BASIC_RENDERER_GLYPH_WIDTH; x++) {
			if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0) {
				*(unsigned int*)(pixPtr + x + (y * this->TargetFramebuffer->PixelsPerScanLine)) = color;
			}
		}
		fontPtr++;
	}

	return;
}

void BasicRenderer::Endl() {
	this->CursorPosition.X = 0;
	this->CursorPosition.Y += STD_BASIC_RENDERER_GLYPH_HIGTH;
	return;
}

void BasicRenderer::ClearLastChar() {
	this->ClearLastChar(this->back_color);
	return;
}

void BasicRenderer::ClearLastChar(uint32_t clear_color) {

	if(this->CursorPosition.X == 0) {
		CursorPosition.X = this->TargetFramebuffer->Width;
		CursorPosition.Y -= STD_BASIC_RENDERER_GLYPH_HIGTH;
		if (this->CursorPosition.Y < 0) CursorPosition.Y = 0;
	}

	for (unsigned long y = this->CursorPosition.Y; y < this->CursorPosition.Y + STD_BASIC_RENDERER_GLYPH_HIGTH; y++) {
		for (unsigned long x = this->CursorPosition.X - STD_BASIC_RENDERER_GLYPH_WIDTH; x < this->CursorPosition.X; x++)
			*(unsigned int*)((unsigned int*)this->TargetFramebuffer->BaseAddress + x + (y * this->TargetFramebuffer->PixelsPerScanLine)) = clear_color;
	}

	this->CursorPosition.X -= STD_BASIC_RENDERER_GLYPH_WIDTH;

	if (this->CursorPosition.X < 0) {
		this->CursorPosition.X = this->TargetFramebuffer->Width;
		this->CursorPosition.Y -= STD_BASIC_RENDERER_GLYPH_HIGTH;
		if (this->CursorPosition.Y < 0) this->CursorPosition.Y = 0;
	}

	return;
}

void BasicRenderer::Clear() {
	this->Clear(this->back_color);
	return;
}

void BasicRenderer::Clear(uint32_t color) {
	uint64_t bPerScanLine = this->TargetFramebuffer->PixelsPerScanLine * 4;

	for (int y = 0; y < this->TargetFramebuffer->Heigth; y++) {
		uint64_t pPtrBase = (uint64_t)this->TargetFramebuffer->BaseAddress + (bPerScanLine * y);
		for (uint32_t* x = (uint32_t*)pPtrBase; x < (uint32_t*)(pPtrBase + bPerScanLine); x++)
			*x = color;
	}

	return;
}
