/*
 * Boot library services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <boot.h>

Status libraryInitialize(LibraryParameters *parameters, BootInfo *bootInfo) {
    Status status;

    consoleDebug(L"Initializing boot library...\r\n");

    bootInfo->signature = BOOT_INFO_SIGNATURE;

    status = mmInitialize(parameters->heapSize);
    if (!STATUS_IS_SUCCESS(status)) {
        return status;
    }

    status = displayInitialize(&bootInfo->fb);
    if (!STATUS_IS_SUCCESS(status)) {
        return status;
    }

    status = fsInitialize();
    if (!STATUS_IS_SUCCESS(status)) {
        return status;
    }

    return STATUS_SUCCESS;
}

void libraryShutdown() {
    fsShutdown();
    mmShutdown();
}
