#include "kernel_util.h"

extern "C" void _start(BootInfo* bootInfo) {

	KernelInfo kernelInfo = InitializeKernel(bootInfo);
	PageTableManager* ptm = kernelInfo.pageTableManager;

	while (true)
	{
		ProcessMousePacket();
	}

	while (true);
}