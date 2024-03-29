#pragma once

#include <stdint.h>
#include "efiMemory.h"

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescrSize);

void memset(void* start, uint8_t value, uint64_t num);

void* memcpy(void* dest, const void* src, uint64_t count);
