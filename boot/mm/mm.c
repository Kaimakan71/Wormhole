/*
 * Boot memory management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <boot.h>
#include <mm.h>

Status mmInitialize(uintn heapSize) {
    Status status;

    consoleDebug(L"Initializing memory manager...\r\n");

    status = mmArchInitialize();
    if (!STATUS_IS_SUCCESS(status)) {
        return status;
    }

    status = mmHeapInitialize(heapSize);
    if (!STATUS_IS_SUCCESS(status)) {
        return status;
    }

    return STATUS_SUCCESS;
}

void mmShutdown() {
    mmHeapShutdown();
    mmArchShutdown();
}
