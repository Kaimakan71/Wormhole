/*
 * Physical memory manager services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <log.h>
#include <mm.h>

#define TRACE_SOURCE "pmm"

Status pmmInitialize(BootMemoryRegion *memoryRegions, uintn memoryRegionCount) {
    BootMemoryRegion *region;

    traceDebug("Initializing PMM...\n");

    region = &memoryRegions[0];
    while (memoryRegionCount--) {
        logDebug("phys 0x%64x virt 0x%64x pageCount %64u type %8u\n", region->physicalAddress, region->virtualAddress, region->pageCount, region->type);
        region++;
    }

    return STATUS_SUCCESS;
}

Status pmmAllocate(uintn pageCount, uintn *physicalAddress) {
    (void) pageCount;
    (void) physicalAddress;

    /* TODO: Implement this routine. */

    return STATUS_ALLOCATION_FAILED;
}

void pmmFree(uintn physicalAddress, uintn pageCount) {
    (void) physicalAddress;
    (void) pageCount;

    /* TODO: Implement this routine. */
}
