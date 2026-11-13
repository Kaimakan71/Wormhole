/*
 * Physical memory manager services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <list.h>
#include <log.h>
#include <mm.h>

#define TRACE_SOURCE "pmm"

#define MM_STATIC_REGION_COUNT 1024

typedef struct {
    ListItem listItem;

    uint64 physicalAddress;
    uint64 pageCount;
} MemoryRegion;

static MemoryRegion mmStaticRegions[MM_STATIC_REGION_COUNT];
static uintn mmStaticRegionsUsed;

static List mmFreeRegions;
static List mmUsedRegions;
static List mmReservedRegions;

static uint64 mmMaximumPfn;
static MemoryRegion **mmPfnTable;

static MemoryRegion* pmmCreateRegion() {
    if (mmStaticRegionsUsed < MM_STATIC_REGION_COUNT) {
        return &mmStaticRegions[mmStaticRegionsUsed++];
    }

    return NULL;
}

Status pmmInitialize(BootMemoryRegion *bootRegions, uintn bootRegionCount) {
    uint64 maxPfn, pfnTablePages, start;
    BootMemoryRegion *bootRegion;
    MemoryRegion *region;

    traceDebug("Initializing PMM...\n");

    /* Initialize state */
    mmStaticRegionsUsed = 0;
    listReset(&mmFreeRegions);
    listReset(&mmUsedRegions);
    listReset(&mmReservedRegions);

    /* Compute maximum PFN */
    mmMaximumPfn = 0;
    bootRegion = &bootRegions[0];
    for (uintn r = 0; r < bootRegionCount; r++) {
        maxPfn = (bootRegion->physicalAddress >> PAGE_SHIFT) + bootRegion->pageCount - 1;
        if (maxPfn > mmMaximumPfn) {
            mmMaximumPfn = maxPfn;
        }

        bootRegion++;
    }

    /* Allocate PFN table */
    pfnTablePages = SIZE_TO_PAGES((mmMaximumPfn + 1) * sizeof(*mmPfnTable));
    mmPfnTable = NULL;
    bootRegion = &bootRegions[0];
    for (uintn r = 0; r < bootRegionCount; r++) {
        if (bootRegion->pageCount >= pfnTablePages) {
            mmPfnTable = physToVirt(bootRegion->physicalAddress);
            bootRegion->physicalAddress += PAGES_TO_SIZE(pfnTablePages);
            bootRegion->pageCount -= pfnTablePages;
            break;
        }

        bootRegion++;
    }
    if (mmPfnTable == NULL) {
        traceError("Failed to allocate %64u page(s) for PFN table\n", pfnTablePages);
        return STATUS_ALLOCATION_FAILED;
    }

    /* Initialize PFN table */
    for (uintn p = 0; p <= mmMaximumPfn; p++) {
        mmPfnTable[p] = NULL;
    }

    /* Process regions */
    bootRegion = &bootRegions[0];
    for (uintn r = 0; r < bootRegionCount; r++) {
        region = pmmCreateRegion();
        if (region == NULL) {
            break;
        }

        /* Add to appropriate list */
        region->physicalAddress = bootRegion->physicalAddress;
        region->pageCount = bootRegion->pageCount;
        if (bootRegion->type == MEMORY_TYPE_FREE) {
            listAppend(&mmFreeRegions, &region->listItem);
        } else {
            listAppend(&mmReservedRegions, &region->listItem);
        }

        /* Register in PFN table */
        start = region->physicalAddress >> PAGE_SHIFT;
        for (uintn p = 0; p < region->pageCount; p++) {
            mmPfnTable[start + p] = region;
        }

        bootRegion++;
    }

    return STATUS_SUCCESS;
}

Status pmmAllocate(uintn pageCount, uintn *physicalAddress) {
    ListItem *listItem;
    MemoryRegion *region, *marginRegion;
    uint64 marginPageCount;

    listEnumerateItems(&mmFreeRegions, listItem) {
        region = (MemoryRegion *) listItem;
        if (region->pageCount < pageCount) {
            continue;
        }

        /* Move region to used list */
        listItemRemove(listItem);
        listAppend(&mmUsedRegions, listItem);

        /* Split remaining pages into seperate region */
        marginPageCount = region->pageCount - pageCount;
        if (marginPageCount > 0) {
            marginRegion = pmmCreateRegion();
            if (marginRegion != NULL) {
                marginRegion->physicalAddress = region->physicalAddress + PAGES_TO_SIZE(pageCount);
                marginRegion->pageCount = marginPageCount;
                listAppend(&mmFreeRegions, &marginRegion->listItem);
            }
        }

        *physicalAddress = region->physicalAddress;
        return STATUS_SUCCESS;
    }

    return STATUS_ALLOCATION_FAILED;
}

void pmmFree(uintn physicalAddress, uintn pageCount) {
    (void) physicalAddress;
    (void) pageCount;

    /* TODO: Implement this routine. */
}
