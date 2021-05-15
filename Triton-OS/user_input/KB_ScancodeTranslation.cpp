#include "KB_ScancodeTranslation.h"

namespace QWERTZKeyboard {

    const char ASCIITable[] = {
         0 ,  0 , '1', '2',     // 0x00 - 0x03
        '3', '4', '5', '6',     // 0x04 - 0x07
        '7', '8', '9', '0',     // 0x08 - 0x0B
         0 , '`',  0 ,  0 ,     // 0x0C - 0x0F
        'q', 'w', 'e', 'r',     // 0x10 - 0x13
        't', 'z', 'u', 'i',     // 0x14 - 0x17
        'o', 'p',  0 , '+',     // 0x18 - 0x1B
       '\n',  0 , 'a', 's',     // 0x1C - 0x1F
        'd', 'f', 'g', 'h',     // 0x20 - 0x23
        'j', 'k', 'l',  0 ,     // 0x24 - 0x27
         0 , '^',  0 , '#',     // 0x28 - 0x2B
        'y', 'x', 'c', 'v',     // 0x2C - 0x2F
        'b', 'n', 'm', ',',     // 0x30 - 0x33
        '.', '-',  0 , '*',     // 0x34 - 0x37
         0 , ' '
    };

    char Translate(uint8_t scancode, bool uppercase) {
        if (scancode > 58) return 0;

        if (uppercase) {
            return ASCIITable[scancode] - 32;
        }
        else return ASCIITable[scancode];
    }
}