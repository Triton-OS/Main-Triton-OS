#pragma once

#include <stdint.h>

enum PT_Flag {
	Present = 0,
	ReadWrite = 1,
	Admin = 2,
	WriteThrough = 3,
	CacheDisabeled = 4,
	Accessed = 5,
	LargerPages = 7,
	Custom0 = 9,
	Custom1 = 10,
	Custom2 = 11,
	NX = 63	// only if supported
};

struct PageDirectoryEntry {

public:

	void SetFlag(PT_Flag flag, bool enabled);
	bool GetFlag(PT_Flag flag);

	void SetAddress(uint64_t address);
	uint64_t GetAddress();

private:

	uint64_t value;
};

struct PageTable {
	PageDirectoryEntry entries[512];
}__attribute__((aligned(0x1000)));