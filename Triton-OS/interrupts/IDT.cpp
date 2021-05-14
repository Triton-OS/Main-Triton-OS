#include "IDT.h"

void IDTDescrEntry::SetOffset(uint64_t offset) {
	this->offset0 = (uint16_t)(offset & 0x000000000000ffff);
	this->offset1 = (uint16_t)((offset & 0x00000000ffff0000) >> 16);
	this->offset2 = (uint32_t)((offset & 0xffffffff00000000) >> 32);
	return;
}

uint64_t IDTDescrEntry::GetOffset() {
	uint64_t offset = 0;
	offset |= (uint64_t)this->offset0;
	offset |= (uint64_t)this->offset1 << 16;
	offset |= (uint64_t)this->offset2 << 32;
	return offset;
}