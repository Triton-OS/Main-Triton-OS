#include "kernel_util.h"

KernelInfo kernelInfo;
PageTableManager ptm = NULL;

void PrepareMemory(BootInfo* bootInfo) {
	uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescriptorSize;

	GlobalAllocator = PageFrameAllocator();
	GlobalAllocator.ReadEfiMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescriptorSize);

	uint64_t kernel_size = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
	uint64_t kernel_pages = (uint64_t)kernel_size / 4096 + 1;

	GlobalAllocator.LockPages(&_KernelStart, kernel_pages);

	PageTable* PML4 = (PageTable*)GlobalAllocator.RequestPage();
	memset(PML4, 0, 0x1000);

	ptm = PageTableManager(PML4);

	for (uint64_t i = 0; i < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescriptorSize); i += 0x1000) ptm.MapMemory((void*)i, (void*)i);

	uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
	uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;

	GlobalAllocator.LockPages((void*)fbBase, fbSize / 0x1000 + 1);

	for (uint64_t i = fbBase; i < fbBase + fbSize; i += 4096) ptm.MapMemory((void*)i, (void*)i);

	for (uint64_t i = fbBase; i < fbBase + fbSize; i += 4096) ptm.MapMemory((void*)i, (void*)i);

	asm("mov %0, %%cr3" : : "r" (PML4));

	kernelInfo.pageTableManager = &ptm;
}

IDTR idtr;

void SetIDTGate(void* handler, uint8_t entryOffset, uint8_t type_attr, uint8_t selector) {
	IDTDescrEntry* interrupt = (IDTDescrEntry*)(idtr.offset + entryOffset * sizeof(IDTDescrEntry));
	interrupt->SetOffset((uint64_t)handler);
	interrupt->type_attr = type_attr;
	interrupt->selector = selector;

	return;
}

void PrepareInterrupts() {
	idtr.limit = 0x0FFF;
	idtr.offset = (uint64_t)GlobalAllocator.RequestPage();

	SetIDTGate((void*)PageFault_Handler, 0x0E, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)DoubleFault_Handler, 0x08, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)GPFault_Handler, 0x0D, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)KeyboardInt_Handler, 0x21, IDT_TA_InterruptGate, 0x08);
	SetIDTGate((void*)MouseInt_Handler, 0x2C, IDT_TA_InterruptGate, 0x08);

	asm ("lidt %0" : : "m" (idtr));

	RemapPIC();
}

BasicRenderer r = BasicRenderer(NULL, NULL);
KernelInfo InitializeKernel(BootInfo* bootInfo) {

	r = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_font);
	GlobalRenderer = &r;

	GDTDescriptor gdtDescriptor;
	gdtDescriptor.size = sizeof(GDT) - 1;
	gdtDescriptor.offset = (uint64_t)&DefaultGDT;
	LoadGDT(&gdtDescriptor);
	
	PrepareMemory(bootInfo);
	
	memset(bootInfo->framebuffer->BaseAddress, 0, bootInfo->framebuffer->BufferSize);	// Clear Screen
	
	PrepareInterrupts();

	PS2InitMouse();

	outb(PIC1_DATA, 0b11111001);
	outb(PIC2_DATA, 0b11101111);

	asm("sti");
	
	return kernelInfo;
}