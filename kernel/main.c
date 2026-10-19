/*
 * Kernel main routine.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <bootinfo.h>
#include <console.h>
#include <cpu.h>
#include <log.h>
#include <mm.h>

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

    logWrite("Goodbye!\n");
    cpuHang();
}
