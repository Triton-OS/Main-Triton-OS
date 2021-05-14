#pragma once
#include <stdint.h>

namespace QWERTZKeyboard {

	extern const char ASCIITable[];
	char Translate(uint8_t scancode, bool uppercase);
};