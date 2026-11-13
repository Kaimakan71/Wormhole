/*
 * Memory management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <bootinfo.h>
#include <cpu/mm.h>
#include <status.h>

#define PAGE_SIZE  4096
#define PAGE_SHIFT 12
#define PAGE_MASK  0xfff

#define SIZE_TO_PAGES(size)  (((size) >> PAGE_SHIFT) + (((size) & PAGE_MASK) ? 1 : 0))
#define PAGES_TO_SIZE(pages) ((pages) << PAGE_SHIFT)

static inline void *physToVirt(uintn phys) {
    return (void *) (phys + HHDM_OFFSET);
}

static inline uintn virtToPhys(void *virt) {
    return (uintn) virt - HHDM_OFFSET;
}

Status mmInitialize(BootMemoryRegion *memoryRegions, uintn memoryRegionCount);

Status pmmInitialize(BootMemoryRegion *memoryRegions, uintn memoryRegionCount);
Status pmmAllocatePages(uintn pageCount, uint64 *physicalAddress);
void pmmFreePages(uint64 physicalAddress, uintn pageCount);
void *pmmGetOwner(uint64 physicalAddress);
void pmmSetOwner(uint64 physicalAddress, void *owner);

Status vmmAllocatePages(uintn pageCount, void **ptr);

static inline void vmmFreePages(void *ptr, uintn pageCount) {
    pmmFreePages(virtToPhys(ptr), pageCount);
}

static inline void *vmmGetOwner(void *ptr) {
    return pmmGetOwner(virtToPhys(ptr));
}

static inline void vmmSetOwner(void *ptr, void *owner) {
    pmmSetOwner(virtToPhys(ptr), owner);
}

Status heapInitialize();
void *heapAllocate(uintn size);
void heapFree(void *ptr);
