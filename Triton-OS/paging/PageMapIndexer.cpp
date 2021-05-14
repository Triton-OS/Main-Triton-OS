#include "PageMapIndexer.h"

PageMapIndexer::PageMapIndexer(uint64_t virtualAddress) {
	virtualAddress >>= 12;
	this->P_i = virtualAddress & 0x1ff;
	virtualAddress >>= 9;
	this->PT_i = virtualAddress & 0x1ff;
	virtualAddress >>= 9;
	this->PD_i = virtualAddress & 0x1ff;
	virtualAddress >>= 9;
	this->PDP_i = virtualAddress & 0x1ff;
}