/*
 * Memory manager services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <bootinfo.h>
#include <status.h>

#define PAGE_SIZE  4096
#define PAGE_SHIFT 12
#define PAGE_MASK  0xfff

Status mmInitialize(MemoryRegion *memoryRegions, uintn memoryRegionCount);

Status pmmInitialize(MemoryRegion *memoryRegions, uintn memoryRegionCount);
