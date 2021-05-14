#pragma once

#include "paging.h"

class PageTableManager {

public:

	PageTable* PML4;

	PageTableManager(PageTable* PML4Address);
	void MapMemory(void* virualMemory, void* physicalMemory);
};