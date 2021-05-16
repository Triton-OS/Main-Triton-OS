#include "FaultScreen.h"

const char* error = "Es ist ein unerwarteter Fehler aufgetreten: \"";

void ShowFaultMessage(const char* header, const char* description, uint16_t fault_code) {

	GlobalRenderer->Clear(0x00A52A2A);

	GlobalRenderer->CursorPosition.Y = GlobalRenderer->TargetFramebuffer->Heigth / 50;
	GlobalRenderer->CursorPosition.X = (GlobalRenderer->TargetFramebuffer->Width / 2) - ((get_length_of(header) + get_length_of(error) + 1) / 2 * STD_BASIC_RENDERER_GLYPH_WIDTH);
	GlobalRenderer->Print(error); GlobalRenderer->Print(header); GlobalRenderer->PutChar('\"');

	GlobalRenderer->Print("\n\n\n\n\r");

	GlobalRenderer->Print(description);

	GlobalRenderer->Print("\n\n\r");

	GlobalRenderer->Print("fault-code: 0x"); GlobalRenderer->Print(to_hexString(fault_code));


	while(true) asm("hlt");
}