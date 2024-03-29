#include "IO.h"

void outb(uint16_t port, uint8_t value) {
	asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
	uint8_t retv;
	asm volatile ("inb %1, %0"
	: "=a"(retv)
	: "Nd"(port));
	return retv;
}

void io_wait() {
	asm volatile ("outb %%al, $0x80" : : "a"(0));
}
