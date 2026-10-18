/*
 * Boot manager main routine.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <boot.h>

#define HEAP_SIZE 0x4000

#define KERNEL_PATH L"\\kernel.elf"

Status libInitialize() {
    Status status;

    status = mmInitialize(HEAP_SIZE);
    if (!STATUS_IS_SUCCESS(status)) {
        return status;
    }

    status = fsInitialize();
    if (!STATUS_IS_SUCCESS(status)) {
        return status;
    }

    return STATUS_SUCCESS;
}

void libShutdown() {
    fsShutdown();
    mmShutdown();
}

Status bootMain() {
    Status status;
    void *kernel;
    uintn kernelSize, kernelEntry;

    consoleDebug(L"Starting boot manager...\r\n");

    status = libInitialize();
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

    libShutdown();
    consoleDebug(L"Kernel entry point: 0x%x\r\n", kernelEntry);

    return STATUS_SUCCESS;
}
