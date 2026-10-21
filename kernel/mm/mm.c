/*
 * Memory manager services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <log.h>
#include <mm.h>

#define TRACE_SOURCE "mm"

Status mmInitialize(MemoryRegion *memoryRegions, uintn memoryRegionCount) {
    Status status;

    traceDebug("Initializing memory manager...\n");

    status = pmmInitialize(memoryRegions, memoryRegionCount);
    if (!STATUS_IS_SUCCESS(status)) {
        return status;
    }

    status = mmHeapInitialize();
    if (!STATUS_IS_SUCCESS(status)) {
        return status;
    }

    return STATUS_SUCCESS;
}

void *mmAllocate(uintn size) {
    return mmHeapAllocate(size);
}

void mmFree(void *ptr) {
    mmHeapFree(ptr);
}
