/*
 * Boot manager main routine.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <boot.h>
#include <cpu.h>

#define HEAP_SIZE 0x4000

#define KERNEL_PATH L"\\kernel.elf"

Status libInitialize(BootInfo *bootInfo) {
    Status status;

    bootInfo->signature = BOOT_INFO_SIGNATURE;

    status = mmInitialize(HEAP_SIZE);
    if (!STATUS_IS_SUCCESS(status)) {
        return status;
    }

    status = fsInitialize();
    if (!STATUS_IS_SUCCESS(status)) {
        return status;
    }

    status = displayInitialize(&bootInfo->fb);
    if (!STATUS_IS_SUCCESS(status)) {
        return status;
    }

    return STATUS_SUCCESS;
}

void libShutdown() {
    displayShutdown();
    fsShutdown();
    mmShutdown();
}

Status bootMain() {
    Status status;
    BootInfo bootInfo;
    void *kernel;
    uintn kernelSize, kernelEntry;
    __sysv_abi void (*kernelMain)(BootInfo *);

    consoleDebug(L"Starting boot manager...\r\n");

    status = libInitialize(&bootInfo);
    if (!STATUS_IS_SUCCESS(status)) {
        libShutdown();
        return status;
    }

    status = fsLoad(KERNEL_PATH, &kernel, &kernelSize);
    if (!STATUS_IS_SUCCESS(status)) {
        libShutdown();
        return status;
    }

    status = imgLoad(kernel, kernelSize, &kernelEntry);
    mmHeapFree(kernel);
    if (!STATUS_IS_SUCCESS(status)) {
        libShutdown();
        return status;
    }

    /*
     * This is the point of no return.
     * Once we disable firmware services, we must either
     * transition to the kernel or hang.
     */

    status = fwShutdown();
    if (!STATUS_IS_SUCCESS(status)) {
        libShutdown();
        return status;
    }

    mmActivateMappings();
    kernelMain = (void *) kernelEntry;
    kernelMain(&bootInfo);
    cpuHang();
}
