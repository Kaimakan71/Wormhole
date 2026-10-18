/*
 * Kernel main routine.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <bootinfo.h>
#include <console.h>
#include <cpu.h>

void kernelMain(BootInfo *bootInfo) {
    if (bootInfo->signature != BOOT_INFO_SIGNATURE) {
        return;
    }

    consoleInitialize(&bootInfo->fb);
    consoleWrite("Hello, Wormhole!\n");
    cpuHang();
}
