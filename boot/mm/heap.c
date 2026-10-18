/*
 * Boot heap services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <boot.h>
#include <mm.h>
#include <tlsf.h>

static uintn heapPageCount;
static void *heapMemory = NULL;
static void *tlsf;

Status mmHeapInitialize(uintn heapSize) {
    Status status;

    consoleDebug(L"Initializing heap...\r\n");

    heapPageCount = SIZE_TO_PAGES(heapSize);
    status = mmAllocatePages(heapPageCount, false, &heapMemory);
    if (!STATUS_IS_SUCCESS(status)) {
        return status;
    }

    tlsf = tlsfInitialize(heapMemory, heapSize);
    if (tlsf == NULL) {
        return STATUS_UNKNOWN_ERROR;
    }

    return STATUS_SUCCESS;
}

void *mmHeapAllocate(uintn size) {
    return tlsfAllocate(tlsf, size);
}

void mmHeapFree(void *ptr) {
    tlsfFree(tlsf, ptr);
}

void mmHeapShutdown() {
    if (heapMemory != NULL) {
        mmFreePages(heapMemory, heapPageCount);
        heapMemory = NULL;
    }
}
