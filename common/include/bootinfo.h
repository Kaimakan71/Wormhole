/*
 * Boot protocol definitions.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

typedef enum {
    PIXEL_FORMAT_R8G8B8X8,
    PIXEL_FORMAT_B8G8R8X8,
} PixelFormat;

typedef struct {
    uint32 *base;
    uint32 width;
    uint32 height;
    uint32 pitch;
    PixelFormat pixelFormat;
} Framebuffer;

#define MEMORY_TYPE_FREE       0
#define MEMORY_TYPE_FIRMWARE   1
#define MEMORY_TYPE_PERSISTENT 2
#define MEMORY_TYPE_RESERVED   0xff

typedef struct {
    uint64 physicalAddress;
    uint64 virtualAddress;
    uintn pageCount;
    uint8 type;
} MemoryRegion;

#define BOOT_INFO_SIGNATURE 0x4f464e49544f4f42 /* "BOOTINFO" */

typedef struct {
    uint64 signature;

    Framebuffer fb;

    uintn memoryRegionCount;
    MemoryRegion *memoryRegions;
} BootInfo;
