/*
 * x86-64 boot memory management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <boot.h>
#include <cpu.h>
#include <mm.h>
#include <pagemap.h>

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

Status mmMapPages(uintn virtualAddress, uintn physicalAddress, uintn pageCount) {
    PagemapCache cache;
    uint16 pml4Index, pdptIndex, pdIndex, ptIndex;

    cache.pml4Index = 0xffff;
    cache.pdptIndex = 0xffff;
    cache.pdIndex = 0xffff;
    while (pageCount--) {
        pml4Index = getPagemapIndex(virtualAddress, 4);
        pdptIndex = getPagemapIndex(virtualAddress, 3);
        pdIndex = getPagemapIndex(virtualAddress, 2);
        ptIndex = getPagemapIndex(virtualAddress, 1);

        /* Update cache as needed */
        if (pml4Index != cache.pml4Index) {
            cache.pml4Index = pml4Index;
            cache.pdpt = getNextLevel(pml4, pml4Index);

            cache.pdptIndex = pdptIndex;
            cache.pd = getNextLevel(cache.pdpt, pdptIndex);

            cache.pdIndex = pdIndex;
            cache.pt = getNextLevel(cache.pd, pdIndex);
        } else if (pdptIndex != cache.pdptIndex) {
            cache.pdptIndex = pdptIndex;
            cache.pd = getNextLevel(cache.pdpt, pdptIndex);

            cache.pdIndex = pdIndex;
            cache.pt = getNextLevel(cache.pd, pdIndex);
        } else if (pdIndex != cache.pdIndex) {
            cache.pdIndex = pdIndex;
            cache.pt = getNextLevel(cache.pd, pdIndex);
        }
        if (cache.pt == NULL) {
            return STATUS_ALLOCATION_FAILED;
        }

        /* Map individual page */
        cache.pt[ptIndex] = physicalAddress | PAGEMAP_ENTRY_PRESENT | PAGEMAP_ENTRY_WRITABLE;
        physicalAddress += PAGE_SIZE;
        virtualAddress += PAGE_SIZE;
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
