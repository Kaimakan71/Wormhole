/*
 * Memory manager services.
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

#define SIZE_TO_PAGES(size) (((size) >> PAGE_SHIFT) + (((size) & PAGE_MASK) ? 1 : 0))

static inline void *physToVirt(uintn phys) {
    return (void *) (phys + HHDM_BASE);
}

static inline uintn virtToPhys(void *virt) {
    return (uintn) virt - HHDM_BASE;
}

Status mmInitialize(MemoryRegion *memoryRegions, uintn memoryRegionCount);
void *mmAllocate(uintn size);
void mmFree(void *ptr);

Status pmmInitialize(MemoryRegion *memoryRegions, uintn memoryRegionCount);
Status pmmAllocate(uintn pageCount, uintn *physicalAddress);
void pmmFree(uintn physicalAddress, uintn pageCount);

Status mmHeapInitialize();
void *mmHeapAllocate(uintn size);
void mmHeapFree(void *ptr);
