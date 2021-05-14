#include "paging.h"

void PageDirectoryEntry::SetFlag(PT_Flag flag, bool enabled) {
	uint64_t bitSelector = (uint64_t)1 << flag;
	this->value &= ~bitSelector;
	if (enabled) this->value |= bitSelector;

	return;
}

bool PageDirectoryEntry::GetFlag(PT_Flag flag) {
	uint64_t bitSelector = (uint64_t)1 << flag;
	return (this->value & bitSelector) > 0 ? true : false;
}

void PageDirectoryEntry::SetAddress(uint64_t address) {
	address &= 0x000000ffffffffff;
	this->value &= 0xfff0000000000fff;
	this->value |= (address << 12);
}

uint64_t PageDirectoryEntry::GetAddress() {
	return (this->value & 0x000ffffffffff000) >> 12;
}