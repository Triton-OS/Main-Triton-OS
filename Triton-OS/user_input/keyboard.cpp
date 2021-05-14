#include "keyboard.h"

void HandleKeyboard(uint8_t scancode) {

	char ascii = QWERTZKeyboard::Translate(scancode, false);
	if (ascii != 0) GlobalRenderer->Print(ascii);

	return;
}