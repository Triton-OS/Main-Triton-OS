#pragma once
#include <stdint.h>
#include "Framebuffer.h"
#include "Math.h"
#include "Point.h"

class Draw {

private:
	/*Draw();
	~Draw() = default;
	static*/ Framebuffer* TargetFramebuffer;

public:
	Draw(Framebuffer* TargetFramebuffer) { this->TargetFramebuffer = TargetFramebuffer; }
	/*static void Init(Framebuffer* TargetFramebuffer);*/

	/// <summary>
	/// This methode draw an rectangle on the screen.
	/// </summary>
	/// <param name="pos_x">left x beginn-position of the rectangle</param>
	/// <param name="pos_y">left y beginn-position of the rectangle</param>
	/// <param name="width"></param>
	/// <param name="heigth"></param>
	/// <param name="color">the color of the rectangle e. g.: 0xffffffff</param>
	void Rectangle(uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t heigth, uint32_t color);

	/// <summary>
	/// This methode draw an Circle on the screen.
	/// </summary>
	/// <param name="midpoint_x"></param>
	/// <param name="midpoint_y"></param>
	/// <param name="radius"></param>
	/// <param name="color">the color of the circle e. g.: 0xffffffff</param>
	void Circle(uint16_t midpoint_x, uint16_t midpoint_y, uint16_t radius, uint32_t color);

	void Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);

};