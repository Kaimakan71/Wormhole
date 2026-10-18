/*
 * Kernel main routine.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <bootinfo.h>
#include <cpu.h>

#define PIXEL(fb, x, y) ((fb)->base + ((y) * (fb)->pitch) + (x))

static void fillRect(const Framebuffer *fb, uint32 x, uint32 y, uint32 w, uint32 h, uint32 color) {
    uint32 *line;

    line = PIXEL(fb, x, y);
    for (uint32 iy = 0; iy < h; iy++) {
        for (uint32 ix = 0; ix < w; ix++) {
            line[ix] = color;
        }

        line += fb->pitch;
    }
}

void kernelMain(BootInfo *bootInfo) {
    if (bootInfo->signature != BOOT_INFO_SIGNATURE) {
        return;
    }

    for (uintn i = 0; i < 16; i++) {
        fillRect(&bootInfo->fb, i * 16, i * 16, 64, 64, i * 0x111111);
    }

    cpuHang();
}
