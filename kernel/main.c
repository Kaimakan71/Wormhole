/*
 * Kernel main routine.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <bootinfo.h>
#include <bus/pci.h>
#include <console.h>
#include <cpu.h>
#include <log.h>
#include <mm.h>

static void printPciDevice(const PciDevice *dev) {
    logDebug("%8x:%8x.%8u: %s: %16x:%16x\n",
        dev->bus, dev->device, dev->function,
        pciGetClassString(dev->class, dev->subclass),
        dev->vendorId,
        dev->deviceId
    );
}

void kernelMain(BootInfo *bootInfo) {
    Status status;

    if (bootInfo->signature != BOOT_INFO_SIGNATURE) {
        return;
    }

    cpuEarlyInitialize();

    consoleInitialize(&bootInfo->fb);
    logWrite("Starting kernel...\n");

    status = mmInitialize(bootInfo->memoryRegions, bootInfo->memoryRegionCount);
    if (!STATUS_IS_SUCCESS(status)) {
        logError("Failed to initialize memory manager\n");
        return;
    }

    pciInitialize();
    pciEnumerateDevices(printPciDevice);

    logWrite("Goodbye!\n");
    cpuHang();
}
