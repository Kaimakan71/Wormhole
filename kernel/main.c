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
#include <cpu/syscall.h>
#include <device.h>
#include <log.h>
#include <mm.h>
#include <process.h>
#include <sched.h>

static void driversInitialize() {
    pciInitialize();
}

static void initMain() {
    logWrite("Welcome to WormholeOS!\n");
    syscall0(SYSCALL_THREAD_EXIT);
}

void kernelMain(BootInfo *bootInfo) {
    Status status;
    Process *initProcess;

    if (bootInfo->signature != BOOT_INFO_SIGNATURE) {
        return;
    }

    cpuPreInitialize();

    consoleInitialize(&bootInfo->fb);
    logWrite("Starting kernel...\n");

    status = mmInitialize(bootInfo->memoryRegions, bootInfo->memoryRegionCount);
    if (!STATUS_IS_SUCCESS(status)) {
        logError("Failed to initialize memory manager\n");
        return;
    }

    deviceInitialize();

    status = cpuInitialize();
    if (!STATUS_IS_SUCCESS(status)) {
        logError("Failed to initialize CPU\n");
        return;
    }

    status = schedInitialize();
    if (!STATUS_IS_SUCCESS(status)) {
        logError("Failed to initialize scheduler\n");
        return;
    }

    driversInitialize();

    initProcess = processCreate("init", initMain);
    if (initProcess == NULL) {
        logError("Failed to create init process\n");
        return;
    }

    schedRegisterProcess(initProcess);
    schedStart();
}
