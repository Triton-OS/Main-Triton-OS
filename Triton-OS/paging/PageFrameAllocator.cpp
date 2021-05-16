#include "PageFrameAllocator.h"

uint64_t freeMemory, reservedMemory,
		 usedMemory, pageBitmapIndex = 0;
bool Initialized = false;
PageFrameAllocator GlobalAllocator;

void PageFrameAllocator::ReadEfiMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescrSize) {
	
	if (Initialized) return;
	Initialized = true;

	uint64_t mMapEntries = mMapSize / mMapDescrSize;

	void* largestFreeMemSeg = nullptr;
	size_t largestFreeMemSegSize = 0;

	for (int i = 0; i < mMapEntries; i++) {
		EFI_MEMORY_DESCRIPTOR* descr = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescrSize));
		if (descr->type == 7) {
			if (descr->numPages * 4096 > largestFreeMemSegSize) {
				largestFreeMemSeg = descr->physAddress;
				largestFreeMemSegSize = descr->numPages * 4096;
			}
		}
	}

	uint64_t memorySize = GetMemorySize(mMap, mMapEntries, mMapDescrSize);
	freeMemory = memorySize;
	uint64_t bitmapSize = memorySize / 4096 / 8 + 1;

	this->InitBitmap(bitmapSize, largestFreeMemSeg);

	this->LockPages(&this->PageBitmap.Buffer, this->PageBitmap.Size / 4096 + 1);

	for (int i = 0; i < mMapEntries; i++) {
		EFI_MEMORY_DESCRIPTOR* descr = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescrSize));
		// not efiConventionalMemory
		if (descr->type != 7) this->ReservePages(descr->physAddress, descr->numPages);
	}
	return;
}

void* PageFrameAllocator::RequestPage() {
	for (; pageBitmapIndex < this->PageBitmap.Size * 8; pageBitmapIndex++) {
		if (this->PageBitmap[pageBitmapIndex] == true) continue;
		this->LockPage((void*)(pageBitmapIndex * 4096));
		return (void*)(pageBitmapIndex * 4096);
	}
	
	return NULL;	// ERROR
}

void PageFrameAllocator::FreePage(void* address) {
	uint64_t index = (uint64_t)address / 4096;
	if (this->PageBitmap[index] == false) return;
	if (this->PageBitmap.Set(index, false)) {
		freeMemory += 4096;
		usedMemory -= 4096;
		if (pageBitmapIndex > index) pageBitmapIndex = index;
	}

	return;
}

void PageFrameAllocator::FreePages(void* address, uint64_t pageCount) {
	for (int i = 0; i < pageCount; i++)
		this->FreePage((void*)((uint64_t)address + (i * 4096)));

	return;
}

void PageFrameAllocator::LockPage(void* address) {
	uint64_t index = (uint64_t)address / 4096;
	if (this->PageBitmap[index] == true) return;
	if (this->PageBitmap.Set(index, true)) {
		freeMemory -= 4096;
		usedMemory += 4096;
	}

	return;
}

void PageFrameAllocator::LockPages(void* address, uint64_t pageCount) {
	for (int i = 0; i < pageCount; i++)
		this->LockPage((void*)((uint64_t)address + (i * 4096)));

	return;
}

uint64_t PageFrameAllocator::GetFreeRAM() {
	return freeMemory;
}

uint64_t PageFrameAllocator::GetUsedRAM() {
	return usedMemory;
}

uint64_t PageFrameAllocator::GetReservedRAM() {
	return reservedMemory;
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* bufferAddress) {
	this->PageBitmap.Size = bitmapSize;
	this->PageBitmap.Buffer = (uint8_t*)bufferAddress;

	for (int i = 0; i < bitmapSize; i++) {
		*(uint8_t*)(this->PageBitmap.Buffer + i) = 0;
	}

	return;
}

void PageFrameAllocator::ReservePage(void* address) {
	uint64_t index = (uint64_t)address / 4096;
	if (this->PageBitmap[index] == true) return;
	if (this->PageBitmap.Set(index, true)) {
		freeMemory -= 4096;
		reservedMemory += 4096;
	}

	return;
}

void PageFrameAllocator::ReservePages(void* address, uint64_t pageCount) {
	for (int i = 0; i < pageCount; i++)
		this->ReservePage((void*)((uint64_t)address + (i * 4096)));

	return;
}

void PageFrameAllocator::UnReservePage(void* address) {
	uint64_t index = (uint64_t)address / 4096;
	if (this->PageBitmap[index] == false) return;
	if (this->PageBitmap.Set(index, false)) {
		freeMemory += 4096;
		reservedMemory -= 4096;
		if (pageBitmapIndex > index) pageBitmapIndex = index;
	}

	return;
}

void PageFrameAllocator::UnReservePages(void* address, uint64_t pageCount) {
	for (int i = 0; i < pageCount; i++)
		this->UnReservePage((void*)((uint64_t)address + (i * 4096)));

	return;
}
