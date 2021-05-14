#pragma once

#include "../efiMemory.h"
#include "../memory.h"
#include "../Bitmap.h"
#include <stdint.h>

class PageFrameAllocator {

public:
	Bitmap PageBitmap;
	void ReadEfiMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescrSize);
	void* RequestPage();
	void FreePage(void* address);
	void FreePages(void* address, uint64_t pageCount);
	void LockPage(void* address);
	void LockPages(void* address, uint64_t pageCount);
	uint64_t GetFreeRAM();
	uint64_t GetUsedRAM();
	uint64_t GetReservedRAM();

private:
	void InitBitmap(size_t bitmapSize, void* bufferAddress);
	void ReservePage(void* address);
	void ReservePages(void* address, uint64_t pageCount);
	void UnReservePage(void* address);
	void UnReservePages(void* address, uint64_t pageCount);
};

extern PageFrameAllocator GlobalAllocator;