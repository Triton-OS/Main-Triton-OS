#include "mouse.h"

Point MousePosition = Point(0, 0);
Point MousePositionOld = Point(0, 0);

/*uint8_t MousePointer[] = {
	0b10000000, 0b00000000, //  0
	0b11000000, 0b00000000, //  1
	0b10100000, 0b00000000, //  2
	0b10010000, 0b00000000, //  3
	0b10001000, 0b00000000, //  4
	0b10000100, 0b00000000, //  5
	0b10000010, 0b00000000, //  6
	0b10000001, 0b00000000, //  7
	0b10000000, 0b10000000, //  8
	0b10000000, 0b01000000, //  9
	0b10000000, 0b00100000, //  A
	0b10000001, 0b11110000, //  B
	0b10001001, 0b00000000, //  C
	0b10011001, 0b00000000, //  D
	0b10100100, 0b10000000, //  E
	0b11000100, 0b10000000,	//  F
	0b10000010, 0b01000000,	// 10
	0b00000010, 0b01000000,	// 11
	0b00000001, 0b00100000,	// 12
	0b00000001, 0b00100000,	// 13
	0b00000000, 0b11000000	// 14
};*/

uint8_t MousePointer[] = {
	0b10000000, 0b00000000, //  0
	0b11000000, 0b00000000, //  1
	0b11100000, 0b00000000, //  2
	0b11110000, 0b00000000, //  3
	0b11111000, 0b00000000, //  4
	0b11111100, 0b00000000, //  5
	0b11100110, 0b00000000, //  6
	0b11100011, 0b00000000, //  7
	0b11111111, 0b10000000, //  8
	0b11111111, 0b11000000, //  9
	0b11111111, 0b11100000, //  A
	0b11111111, 0b11110000, //  B
	0b11111111, 0b00000000, //  C
	0b11111111, 0b00000000, //  D
	0b11100111, 0b10000000, //  E
	0b11000111, 0b10000000,	//  F
	0b10000011, 0b11000000,	// 10
	0b00000011, 0b11000000,	// 11
	0b00000001, 0b11100000,	// 12
	0b00000001, 0b11100000,	// 13
	0b00000000, 0b11000000	// 14
};

uint8_t mouseCycle = 0;
uint8_t MousePacket[4];
bool mousePacketReady = false;

void MouseWait();
void MouseWaitForInput();
void MouseWrite(uint8_t value);
uint8_t MouseRead();

void PS2InitMouse() {

	outb(0x64, 0xA8);

	MouseWait();
	outb(0x64, 0x20);
	
	MouseWaitForInput();

	uint8_t status = inb(0x60);
	status |= 0b10;

	MouseWait();
	outb(0x64, 0x64);

	MouseWait();
	outb(0x60, status);

	MouseWrite(0xF6);
	MouseRead();

	MouseWrite(0xF4);
	MouseRead();

	return;
}

void HandlePS2Mouse(uint8_t data) {
	if (!mousePacketReady) {
		switch (mouseCycle) {
		case 0:
			if ((data & 0b00001000) == 0) break;
			MousePacket[0] = data;
			mouseCycle++;
			break;
		case 1:
			MousePacket[1] = data;
			mouseCycle++;
			break;
		case 2:
			MousePacket[2] = data;
			mousePacketReady = true;
			mouseCycle = 0;
			break;
		}
	}

	return;
}

void ProcessMousePacket() {
	if (!mousePacketReady) return;
	mousePacketReady = false;

	bool xNegative, xOverflow, yNegative, yOverflow;

	if (MousePacket[0] & PS2XSign) xNegative = true;
	else xNegative = false;

	if (MousePacket[0] & PS2XOverflow) xOverflow = true;
	else xOverflow = false;

	if (MousePacket[0] & PS2YSign) yNegative = true;
	else yNegative = false;

	if (MousePacket[0] & PS2YOverflow) yOverflow = true;
	else yOverflow = false;

	if (!xNegative) {
		MousePosition.X += MousePacket[1];
		if (xOverflow) MousePosition.X += 255;
	}
	else {
		MousePacket[1] = 256 - MousePacket[1];
		MousePosition.X -= MousePacket[1];
		if (xOverflow) MousePosition.X -= 255;
	}

	if (!yNegative) {
		MousePosition.Y -= MousePacket[2];
		if (yOverflow) MousePosition.Y -= 255;
	}
	else {
		MousePacket[2] = 256 - MousePacket[2];
		MousePosition.Y += MousePacket[2];
		if (yOverflow) MousePosition.Y += 255;
	}

	if (MousePosition.X < 0) MousePosition.X = 0;
	else if (MousePosition.X > GlobalRenderer->TargetFramebuffer->Width - 8) MousePosition.X = GlobalRenderer->TargetFramebuffer->Width - 1;
	
	if (MousePosition.Y < 0) MousePosition.Y = 0;
	else if (MousePosition.Y > GlobalRenderer->TargetFramebuffer->Heigth - 18) MousePosition.Y = GlobalRenderer->TargetFramebuffer->Heigth - 1;

	GlobalRenderer->ClearMouseCursor(MousePointer, MousePositionOld);
	GlobalRenderer->DrawOverlayMouseCursor(MousePointer, MousePosition, 0x00FF00);
	
	MousePositionOld = MousePosition;
}

void MouseWait() {
	uint64_t timeout = 100000;
	while (timeout--) {
		if ((inb(0x64) & 0b10) == 0) {
			return;
		}
	}

	return;
}

void MouseWaitForInput() {
	uint64_t timeout = 100000;
	while (timeout--) {
		if (inb(0x64) & 0b1) {
			return;
		}
	}

	return;
}

void MouseWrite(uint8_t value) {
	MouseWait();
	outb(0x64, 0xD4);
	MouseWait();
	outb(0x60, value);

	return;
}

uint8_t MouseRead() {
	MouseWaitForInput();
	return inb(0x60);
}
