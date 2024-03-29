#pragma once

#include <stdint.h>
#include "BasicRenderer.h"
#include "string.h"
#include "efiMemory.h"
#include "memory.h"
#include "Bitmap.h"
#include "IO.h"
#include "paging/paging.h"
#include "paging/PageMapIndexer.h"
#include "paging/PageFrameAllocator.h"
#include "paging/PageTableManager.h"
#include "gdt/gdt.h"
#include "interrupts/IDT.h"
#include "interrupts/interrupts.h"
#include "user_input/mouse.h"

struct BootInfo {
    Framebuffer* framebuffer;
    PSF1_FONT* psf1_font;
    EFI_MEMORY_DESCRIPTOR* mMap;
    uint64_t mMapSize;
    uint64_t mMapDescriptorSize;
};

struct KernelInfo {
    PageTableManager* pageTableManager;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

KernelInfo InitializeKernel(BootInfo* bootInfo);