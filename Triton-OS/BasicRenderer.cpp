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

void BasicRenderer::Print(char chr) {
	if (chr == '\n') { if (this->CursorPosition.Y + 18 < this->TargetFramebuffer->Heigth) this->CursorPosition.Y += 18; }
	else if (chr == '\r') this->CursorPosition.X = 0;
	else {
		this->CursorPosition.X += 8;
		if (this->CursorPosition.X + 8 > this->TargetFramebuffer->Width) {
			if (this->CursorPosition.Y + 18 < this->TargetFramebuffer->Heigth) { this->CursorPosition.X = 0; this->CursorPosition.Y += 18; }
		}
		PutChar(chr, this->CursorPosition.X, this->CursorPosition.Y, color);
	}
}

void BasicRenderer::Print(const char* str) {
	this->Print(str);
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
	else if (c == '\n') { if (this->CursorPosition.Y + 18 < this->TargetFramebuffer->Heigth) this->CursorPosition.Y += 18; }
	else {
		this->CursorPosition.X += 8;
		if (this->CursorPosition.X + 8 > this->TargetFramebuffer->Width) { 
			if (this->CursorPosition.Y + 18 < this->TargetFramebuffer->Heigth) { 
				this->CursorPosition.X = 0; 
				this->CursorPosition.Y += 18; 
			} 
		}
		this->PutChar(c, this->CursorPosition.X, this->CursorPosition.Y, color);
	}
	return;
}

void BasicRenderer::PutChar(char c, uint32_t xOff, uint32_t yOff, uint32_t color) {

	unsigned int* pixPtr = (unsigned int*)this->TargetFramebuffer->BaseAddress;
	char* fontPtr = (char*)this->psf1_font->glyphBuffer + (c * this->psf1_font->psf1_header->charsize);
	
	for (unsigned long y = yOff; y < yOff + 18; y++) {
		for (unsigned long x = xOff; x < xOff + 8; x++) {
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
		if (this->CursorPosition.Y > 0) CursorPosition.Y -= STD_BASIC_RENDERER_GLYPH_WIDTH;
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
