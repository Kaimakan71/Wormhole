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

#define BOOT_INFO_SIGNATURE 0x4f464e49544f4f42 /* "BOOTINFO" */

typedef struct {
    uint64 signature;
    Framebuffer fb;
} BootInfo;
