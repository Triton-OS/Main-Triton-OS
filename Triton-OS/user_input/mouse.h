#pragma once
#include "../IO.h"
#include "../Math.h"
#include "../BasicRenderer.h"

#define PS2LeftButton 0b00000001
#define PS2MiddleButton 0b00000010
#define PS2RigthButton 0b00000100

#define PS2XSign 0b00010000
#define PS2XOverflow 0b01000000
#define PS2YSign 0b00100000
#define PS2YOverflow 0b10000000

extern uint8_t MousePointer[];
extern Point MousePosition;

void PS2InitMouse();
void HandlePS2Mouse(uint8_t data);
void ProcessMousePacket();
