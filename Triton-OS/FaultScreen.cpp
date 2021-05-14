#include "FaultScreen.h"

void ShowFaultMessage(const char* header, const char* description, uint16_t fault_code) {
	const char* error = "Es ist ein unerwarteter Fehler aufgetreten: \"";
	GlobalRenderer->Clear(0x446644);
	GlobalRenderer->CursorPosition.X = (GlobalRenderer->TargetFramebuffer->Width / 2) - ((get_length_of(header) + get_length_of(error) + 1) / 2 * 8);
	GlobalRenderer->CursorPosition.Y = GlobalRenderer->TargetFramebuffer->Heigth / 100;
	GlobalRenderer->color = 0x00aa0001;
	GlobalRenderer->Print(error, Zoom::X3); GlobalRenderer->Print(header, Zoom::X3); GlobalRenderer->Print("\"", Zoom::X3);	// 'Es ist ein unerwarteter Fehler aufgetreten: "fehler"'
	GlobalRenderer->CursorPosition.X = GlobalRenderer->TargetFramebuffer->Width / 50;
	GlobalRenderer->CursorPosition.Y = GlobalRenderer->TargetFramebuffer->Heigth / 20 * 3;
	GlobalRenderer->color = 0x00330000;
	GlobalRenderer->Print("Fehlercode: 0x", Zoom::X2); GlobalRenderer->Print(to_hexString(fault_code), Zoom::X2);	// 'Fehlerode: 0xcode'
	GlobalRenderer->CursorPosition.X = GlobalRenderer->TargetFramebuffer->Width / 50;
	GlobalRenderer->CursorPosition.Y = GlobalRenderer->TargetFramebuffer->Heigth / 20 * 4;
	GlobalRenderer->Print(description, Zoom::X2);	// bla, bla, bla

	asm("hlt");
}

/*void print_error(const char* error_string, unsigned char line, zoom z = zoom::normal) {
	if (line > 15) line = 15;
	GlobalRenderer->CursorPosition.X = (GlobalRenderer->TargetFramebuffer->Width / 2) - (get_length_of(error_string) / 2 * 8) * z;
	GlobalRenderer->CursorPosition.Y = (GlobalRenderer->TargetFramebuffer->Heigth / 2) + (line * 18 * z) - (GlobalRenderer->TargetFramebuffer->Heigth / 3);
	GlobalRenderer->Print(error_string, 0x0, z);
}*/