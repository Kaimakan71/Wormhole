/*
 * Physical memory manager services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <log.h>
#include <mm.h>

#define TRACE_SOURCE "pmm"

Status pmmInitialize(MemoryRegion *memoryRegions, uintn memoryRegionCount) {
    MemoryRegion *region;
    uintn maxPfn, top;

    traceDebug("Initializing PMM...\n");

    region = &memoryRegions[0];
    maxPfn = 0;
    for (uintn r = 0; r < memoryRegionCount; r++) {
        top = (region->physicalAddress >> PAGE_SHIFT) + region->pageCount - 1;
        if (top > maxPfn) {
            maxPfn = top;
        }

        region++;
    }

    traceDebug("Maximum PFN: 0x%x\n", maxPfn);

    return STATUS_SUCCESS;
}
