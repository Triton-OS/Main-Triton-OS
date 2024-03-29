#include "keyboard.h"

bool isLeftShiftPressed;
bool isRightShiftPressed;

void HandleKeyboard(uint8_t scancode) {
    
    switch (scancode) {
    case LeftShift:
        isLeftShiftPressed = true;
        return;
    case LeftShift + 0x80:
        isLeftShiftPressed = false;
        return;
    case RightShift:
        isRightShiftPressed = true;
        return;
    case RightShift + 0x80:
        isRightShiftPressed = false;
        return;
    case Enter:
        GlobalRenderer->Print("\n\r");
        return;
    case Spacebar:
        GlobalRenderer->PutChar(' ');
        return;
    case BackSpace:
        GlobalRenderer->ClearLastChar();
        return;
    }

    char ascii = QWERTZKeyboard::Translate(scancode, isLeftShiftPressed | isRightShiftPressed);

    if (ascii != 0) GlobalRenderer->PutChar(ascii);

    return;
    
}