/*
 * x86-64 boot memory management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <boot.h>
#include <bootmm.h>
#include <cpu.h>
#include <cpu/pagemap.h>

#define PAGEMAP_INDEX_INVALID 0xffff

typedef struct {
    PagemapEntry *pdpt;
    PagemapEntry *pd;
    PagemapEntry *pt;

    uint16 pml4Index;
    uint16 pdptIndex;
    uint16 pdIndex;
} PagemapCache;

static PagemapEntry *pml4 = NULL;

static inline uint16 getPagemapIndex(uintn virtualAddress, uint8 level) {
    return (virtualAddress >> (PAGE_SHIFT + (PAGEMAP_ENTRY_SHIFT * (level - 1)))) & PAGEMAP_ENTRY_MASK;
}

static PagemapEntry *getNextLevel(PagemapEntry *table, uint16 index) {
    Status status;
    PagemapEntry *newTable, entry;

    if (table == NULL) {
        return NULL;
    }

    entry = table[index];
    if (entry & PAGEMAP_ENTRY_PRESENT) {
        return (PagemapEntry *) (entry & PAGEMAP_ENTRY_ADDRESS_MASK);
    }

    status = mmAllocatePages(1, false, (void **) &newTable);
    if (!STATUS_IS_SUCCESS(status)) {
        consoleError(L"Failed to allocate page for page table\r\n");
        return NULL;
    }

    for (uintn e = 0; e < PAGEMAP_ENTRY_COUNT; e++) {
        newTable[e] = 0;
    }

    table[index] = (uintn) newTable | PAGEMAP_ENTRY_PRESENT | PAGEMAP_ENTRY_WRITABLE;

    return newTable;
}

Status mmArchInitialize() {
    Status status;

    status = mmAllocatePages(1, false, (void **) &pml4);
    if (!STATUS_IS_SUCCESS(status)) {
        consoleError(L"Failed to allocate page for PML4\r\n");
        return status;
    }

    for (uintn e = 0; e < PAGEMAP_ENTRY_COUNT; e++) {
        pml4[e] = 0;
    }

    return STATUS_SUCCESS;
}

uintn mmArchGetStack() {
    return cpuGetRsp();
}

Status mmMapPages(uintn virtualAddress, uintn physicalAddress, uintn pageCount) {
    PagemapCache cache;
    uint16 pml4Index, pdptIndex, pdIndex, ptIndex;
    PagemapEntry entryFlags;

    entryFlags = PAGEMAP_ENTRY_PRESENT | PAGEMAP_ENTRY_WRITABLE;

    cache.pml4Index = PAGEMAP_INDEX_INVALID;
    while (pageCount) {
        pml4Index = getPagemapIndex(virtualAddress, 4);
        if (pml4Index != cache.pml4Index) {
            cache.pml4Index = pml4Index;
            cache.pdptIndex = PAGEMAP_INDEX_INVALID;
            cache.pdpt = getNextLevel(pml4, pml4Index);
            if (cache.pdpt == NULL) {
                return STATUS_ALLOCATION_FAILED;
            }
        }

        pdptIndex = getPagemapIndex(virtualAddress, 3);
        if (pdptIndex != cache.pdptIndex) {
            cache.pdptIndex = pdptIndex;
            cache.pdIndex = PAGEMAP_INDEX_INVALID;
            cache.pd = getNextLevel(cache.pdpt, pdptIndex);
            if (cache.pd == NULL) {
                return STATUS_ALLOCATION_FAILED;
            }
        }

        pdIndex = getPagemapIndex(virtualAddress, 2);

        /* Map huge page (2 MiB) */
        if ((virtualAddress % HUGE_PAGE_SIZE) == 0 &&
            pageCount >= HUGE_PAGE_PAGES) {
            cache.pd[pdIndex] = entryFlags | physicalAddress | PAGEMAP_ENTRY_PAGE_SIZE;
            physicalAddress += HUGE_PAGE_SIZE;
            virtualAddress += HUGE_PAGE_SIZE;
            pageCount -= HUGE_PAGE_PAGES;
            continue;
        }

        if (pdIndex != cache.pdIndex) {
            cache.pdIndex = pdIndex;
            cache.pt = getNextLevel(cache.pd, pdIndex);
            if (cache.pt == NULL) {
                return STATUS_ALLOCATION_FAILED;
            }
        }

        /* Map single page (4 KiB) */
        ptIndex = getPagemapIndex(virtualAddress, 1);
        cache.pt[ptIndex] = entryFlags | physicalAddress;
        physicalAddress += PAGE_SIZE;
        virtualAddress += PAGE_SIZE;
        pageCount--;
    }

    return STATUS_SUCCESS;
}

Status mmGetPhysicalAddress(uintn virtualAddress, uintn *physicalAddress) {
    PagemapEntry *pdpt, *pd, *pt, pml4Entry, pdptEntry, pdEntry, ptEntry;
    uint16 pageOff, pml4Index, pdptIndex, pdIndex, ptIndex;

    pageOff = virtualAddress & PAGE_MASK;
    virtualAddress &= ~PAGE_MASK;
    pml4Index = getPagemapIndex(virtualAddress, 4);
    pdptIndex = getPagemapIndex(virtualAddress, 3);
    pdIndex = getPagemapIndex(virtualAddress, 2);
    ptIndex = getPagemapIndex(virtualAddress, 1);

    pml4Entry = pml4[pml4Index];
    pdpt = (PagemapEntry *) (pml4Entry & PAGEMAP_ENTRY_ADDRESS_MASK);
    if (!(pml4Entry & PAGEMAP_ENTRY_PRESENT)) {
        return STATUS_NOT_FOUND;
    }

    pdptEntry = pdpt[pdptIndex];
    pd = (PagemapEntry *) (pdptEntry & PAGEMAP_ENTRY_ADDRESS_MASK);
    if (!(pdptEntry & PAGEMAP_ENTRY_PRESENT)) {
        return STATUS_NOT_FOUND;
    }

    pdEntry = pd[pdIndex];
    pt = (PagemapEntry *) (pdEntry & PAGEMAP_ENTRY_ADDRESS_MASK);
    if (!(pdEntry & PAGEMAP_ENTRY_PRESENT)) {
        return STATUS_NOT_FOUND;
    }

    ptEntry = pt[ptIndex];
    if (!(ptEntry & PAGEMAP_ENTRY_PRESENT)) {
        return STATUS_NOT_FOUND;
    }

    *physicalAddress = (ptEntry & PAGEMAP_ENTRY_ADDRESS_MASK) | pageOff;
    return STATUS_SUCCESS;
}

void mmActivateMappings() {
    cpuSetCr3((uint64) pml4);
}

void mmArchShutdown() {
    if (pml4 != NULL) {
        mmFreePages(pml4, 1);
        pml4 = NULL;
    }
}
