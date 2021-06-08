#include "kernel_util.h"

extern "C" void _start(BootInfo* bootInfo) {

	KernelInfo kernelInfo = InitializeKernel(bootInfo);
	PageTableManager* ptm = kernelInfo.pageTableManager;
	GlobalRenderer->Clear(0x000000);
	GlobalRenderer->SetColor(0x006400);
	GlobalRenderer->Print("Hello Triton-OS!");

	while (true) {
		ProcessMousePacket();
	}

	while (true);
}