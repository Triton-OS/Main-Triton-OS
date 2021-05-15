#include "kernel_util.h"

extern "C" void _start(BootInfo* bootInfo) {

	KernelInfo kernelInfo = InitializeKernel(bootInfo);
	PageTableManager* ptm = kernelInfo.pageTableManager;

	GlobalRenderer->Print("PlanetNine[OS] erfolgreich geladen!");

	while (true);
}