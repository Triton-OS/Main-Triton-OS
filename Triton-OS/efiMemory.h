#pragma once

#include <stdint.h>

struct EFI_MEMORY_DESCRIPTOR {
	uint32_t type;
	void* physAddress;
	void* virtAddress;
	uint64_t numPages;
	uint64_t attributes;
};

extern const char* EFI_MEMORY_TYPE_STRINGS[];
