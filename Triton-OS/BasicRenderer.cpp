#include "BasicRenderer.h"
#include <string>

BasicRenderer* GlobalRenderer;

BasicRenderer::BasicRenderer(Framebuffer* TargetFramebuffer, PSF1_FONT* psf1_font, uint32_t color, Zoom zoom) {
	this->TargetFramebuffer = TargetFramebuffer;
	this->CursorPosition = { 0, 0 };
	this->psf1_font = psf1_font;
	this->color = color;
	this->zoom = zoom;
}

void BasicRenderer::SetColor(unsigned int color) {
	this->color = color;
	return;
}

void BasicRenderer::SetZoom(Zoom z) {
	this->zoom = z;
	return;
}

void BasicRenderer::Print(char chr, Zoom z) {
	if (chr == '\n') { if (this->CursorPosition.Y + 18 < this->TargetFramebuffer->Heigth) this->CursorPosition.Y += 18; }
	else if (chr == '\r') this->CursorPosition.X = 0;
	else {
		this->CursorPosition.X += 8;
		if (this->CursorPosition.X + 8 > this->TargetFramebuffer->Width) {
			if (this->CursorPosition.Y + 18 < this->TargetFramebuffer->Heigth) { this->CursorPosition.X = 0; this->CursorPosition.Y += 18; }
		}
		PutChar(chr, this->CursorPosition.X, this->CursorPosition.Y, color, z);
	}
}

void BasicRenderer::Print(const char* str, Zoom z) {

	this->Print(str, this->color, z);
	return;
}

void BasicRenderer::Print(const char* str, unsigned int color, Zoom z) {

	char* c = (char*)str;
	uint8_t zoom;
	switch (z) { case Zoom::Normal || Zoom::X1 || Zoom::X2 || Zoom::X3: zoom = 1; break; default: zoom = 2; }

	while (*c != '\0') {
		if (*c == '\n') { if (this->CursorPosition.Y + 18 < this->TargetFramebuffer->Heigth) this->CursorPosition.Y += 18; }
		else if (*c == '\r') this->CursorPosition.X = 0;
		else {
			this->CursorPosition.X += 8;
			if (this->CursorPosition.X + 8 > this->TargetFramebuffer->Width) {
				if (this->CursorPosition.Y + 18 < this->TargetFramebuffer->Heigth) { this->CursorPosition.X = 0; this->CursorPosition.Y += 18; }
			}
			PutChar(*c, this->CursorPosition.X, this->CursorPosition.Y, color, z);
		}
		c++;
	}

	return;
}

void BasicRenderer::PutChar(char c, unsigned int xOff, unsigned int yOff, unsigned int color, Zoom z) {

	unsigned int* pixPtr = (unsigned int*)this->TargetFramebuffer->BaseAddress;
	char* fontPtr = (char*)this->psf1_font->glyphBuffer + (c * this->psf1_font->psf1_header->charsize);
	
	for (unsigned long y = yOff; y < yOff + (18 * z); y++) {
		for (unsigned long x = xOff; x < xOff + 10; x++) {
			if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0) {
				for(int j = x * z; j < x * z + z; j++)
				*(unsigned int*)(pixPtr + j + (y * this->TargetFramebuffer->PixelsPerScanLine)) = color;
			}
		}
		if (y % z == 0)fontPtr++;
	}

	return;
}

void BasicRenderer::Clear() {
	this->Clear(this->color);
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
