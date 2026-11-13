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
#include <process.h>

static void driversInitialize() {
    pciInitialize();
}

static void idle() {
    for (;;) {
        cpuHalt();
    }
}

void kernelMain(BootInfo *bootInfo) {
    Status status;
    Process *idleProcess;

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

    cpuInitialize();

    driversInitialize();

    logDebug("Creating idle process...\n");
    idleProcess = processCreate("idle");
    if (idleProcess == NULL) {
        logError("Failed to create idle process\n");
        return;
    }

    logWrite("Idling...\n");
    idle();
}
