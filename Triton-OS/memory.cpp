#include "memory.h"

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescrSize) {
	
	static uint64_t mSizeBytes = 0;
	if (mSizeBytes > 0) return mSizeBytes;

	for (int i = 0; i < mMapEntries; i++) {
		EFI_MEMORY_DESCRIPTOR* descr = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescrSize));
		mSizeBytes += descr->numPages * 4096;
	}

	return mSizeBytes;
}

void memset(void* start, uint8_t value, uint64_t num) {
	for (uint64_t i = 0; i < num; i++)
		*(uint8_t*)((uint64_t)start + i) = value;	
}
