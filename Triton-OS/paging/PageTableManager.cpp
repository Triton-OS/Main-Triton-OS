#include "PageTableManager.h"
#include "PageMapIndexer.h"
#include "PageFrameAllocator.h"
#include "../memory.h"
#include <stdint.h>

PageTableManager::PageTableManager(PageTable* PML4Address) {
	this->PML4 = PML4Address;
}

void PageTableManager::MapMemory(void* virualMemory, void* physicalMemory) {
	
	PageMapIndexer indexer = PageMapIndexer((uint64_t)virualMemory);
	PageDirectoryEntry PDE;
	

	// PDP

	PDE = this->PML4->entries[indexer.PDP_i];
	PageTable* PDP;

	if (!PDE.GetFlag(PT_Flag::Present)) {
		PDP = (PageTable*)GlobalAllocator.RequestPage();
		memset(PDP, 0, 0x1000);
		PDE.SetAddress((uint64_t)PDP >> 12);
		PDE.SetFlag(PT_Flag::Present, true);
		PDE.SetFlag(PT_Flag::ReadWrite, true);
		this->PML4->entries[indexer.PDP_i] = PDE;
	} else {
		PDP = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
	}


	// PD (Page Directory)

	PDE = PDP->entries[indexer.PD_i];
	PageTable* PD;

	if (!PDE.GetFlag(PT_Flag::Present)) {
		PD = (PageTable*)GlobalAllocator.RequestPage();
		memset(PD, 0, 0x1000);
		PDE.SetAddress((uint64_t)PD >> 12);
		PDE.SetFlag(PT_Flag::Present, true);
		PDE.SetFlag(PT_Flag::ReadWrite, true);
		PDP->entries[indexer.PD_i] = PDE;
	}
	else {
		PD = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
	}


	// PT (Page Table)

	PDE = PD->entries[indexer.PT_i];
	PageTable* PT;

	if (!PDE.GetFlag(PT_Flag::Present)) {
		PT = (PageTable*)GlobalAllocator.RequestPage();
		memset(PT, 0, 0x1000);
		PDE.SetAddress((uint64_t)PT >> 12);
		PDE.SetFlag(PT_Flag::Present, true);
		PDE.SetFlag(PT_Flag::ReadWrite, true);
		PD->entries[indexer.PT_i] = PDE;
	}
	else {
		PT = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
	}


	PDE = PT->entries[indexer.P_i];
	PDE.SetAddress((uint64_t)physicalMemory >> 12);
	PDE.SetFlag(PT_Flag::Present, true);
	PDE.SetFlag(PT_Flag::ReadWrite, true);
	PT->entries[indexer.P_i] = PDE;

	return;
}