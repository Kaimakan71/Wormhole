/*
 * Console services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <console.h>
#include <font.h>

#define DEFAULT_FG 0xaaaaaa
#define DEFAULT_BG 0x000000

#define PIXEL(fb, x, y) ((fb)->base + ((y) * (fb)->pitch) + (x))

static Framebuffer *fb = NULL;
static uint32 fg;
static uint32 bg;
static uint32 cursorX;
static uint32 cursorY;

static void writeCharacter(char8 c) {
    uint8 *src;
    uint32 *dest;

    src = &fontData[c * FONT_HEIGHT];
    dest = PIXEL(fb, cursorX, cursorY);
    for (uint32 iy = 0; iy < FONT_HEIGHT; iy++) {
        for (uint32 ix = 0; ix < FONT_WIDTH; ix++) {
            if (*src & BIT(FONT_WIDTH - ix)) {
                dest[ix] = fg;
            } else {
                dest[ix] = bg;
            }
        }

        src++;
        dest += fb->pitch;
    }

    cursorX += FONT_WIDTH;
}

void consoleInitialize(Framebuffer *newFb) {
    fb = newFb;
    consoleReset(true);
}

void consoleReset(bool clear) {
    uint32 *line;

    fg = DEFAULT_FG;
    bg = DEFAULT_BG;
    cursorX = 0;
    cursorY = 0;

    if (clear && fb != NULL) {
        line = PIXEL(fb, 0, 0);
        for (uint32 y = 0; y < fb->height; y++) {
            for (uint32 x = 0; x < fb->width; x++) {
                line[x] = bg;
            }

            line += fb->pitch;
        }
    }
}

void consoleSetColor(uint32 color) {
    fg = color;
}

void consoleWrite(const char8 *s) {
    if (fb == NULL) {
        return;
    }

    while (*s != '\0') {
        if (*s == '\n') {
            cursorX = 0;
            cursorY += FONT_HEIGHT;
        } else {
            writeCharacter(*s);
        }

        s++;
    }
}
