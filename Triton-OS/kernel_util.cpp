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

	for (uint64_t i = 0; i < GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescriptorSize); i += 0x1000) {
		ptm.MapMemory((void*)i, (void*)i);
	}

	uint64_t fbBase = (uint64_t)bootInfo->framebuffer->BaseAddress;
	uint64_t fbSize = (uint64_t)bootInfo->framebuffer->BufferSize + 0x1000;

	GlobalAllocator.LockPages((void*)fbBase, fbSize / 0x1000 + 1);

	for (uint64_t i = fbBase; i < fbBase + fbSize; i += 4096) {
		ptm.MapMemory((void*)i, (void*)i);
	}

	for (uint64_t i = fbBase; i < fbBase + fbSize; i += 4096)
		ptm.MapMemory((void*)i, (void*)i);

	asm("mov %0, %%cr3" : : "r" (PML4));

	kernelInfo.pageTableManager = &ptm;
}

IDTR idtr;
void PrepareInterrupts() {
	idtr.limit = 0x0FFF;
	idtr.offset = (uint64_t)GlobalAllocator.RequestPage();

	#pragma region Handler
	IDTDescrEntry* int_PageFault = (IDTDescrEntry*)(idtr.offset + 0xE * sizeof(IDTDescrEntry));
	int_PageFault->SetOffset((uint64_t)PageFault_Handler);
	int_PageFault->type_attr = IDT_TA_InterruptGate;
	int_PageFault->selector = 0x08;

	IDTDescrEntry* int_DoubleFault = (IDTDescrEntry*)(idtr.offset + 0x8 * sizeof(IDTDescrEntry));
	int_DoubleFault->SetOffset((uint64_t)DoubleFault_Handler);
	int_DoubleFault->type_attr = IDT_TA_InterruptGate;
	int_DoubleFault->selector = 0x08;

	IDTDescrEntry* int_GPFault = (IDTDescrEntry*)(idtr.offset + 0xD * sizeof(IDTDescrEntry));
	int_GPFault->SetOffset((uint64_t)GPFault_Handler);
	int_GPFault->type_attr = IDT_TA_InterruptGate;
	int_GPFault->selector = 0x08;

	IDTDescrEntry* int_Keyboard = (IDTDescrEntry*)(idtr.offset + 0x21 * sizeof(IDTDescrEntry));
	int_Keyboard->SetOffset((uint64_t)KeyboarInt_Handler);
	int_Keyboard->type_attr = IDT_TA_InterruptGate;
	int_Keyboard->selector = 0x08;
	#pragma endregion

	asm ("lidt %0" : : "m" (idtr));

	#pragma region Keyboard
	RemapPIC();
	outb(PIC1_DATA, 0b11111101);
	outb(PIC2_DATA, 0b11111111);

	asm("sti");
	#pragma endregion

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
	
	return kernelInfo;
}