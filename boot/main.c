/*
 * Boot manager main routine.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <boot.h>

#define HEAP_SIZE 0x4000

Status libInitialize() {
    Status status;

    status = mmHeapInitialize(HEAP_SIZE);
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
    mmHeapShutdown();
}

Status bootMain() {
    Status status;

    consoleDebug(L"Starting boot manager...\r\n");

    status = libInitialize();
    if (!STATUS_IS_SUCCESS(status)) {
        libShutdown();
        return status;
    }

    libShutdown();

    return STATUS_SUCCESS;
}
