/*
 * Memory manipulation services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

static inline void memFill(void *dest, uint8 value, uintn count) {
    uint8 *d;

    d = dest;
    while (count--) {
        *d++ = value;
    }
}

static inline void memClear(void *dest, uintn count) {
    memFill(dest, 0, count);
}

static inline void memCopy(void *dest, void *src, uintn count) {
    uint8 *d, *s;

    d = dest;
    s = src;
    while (count--) {
        *d++ = *s++;
    }
}

static inline void memMove(void *dest, void *src, uintn count) {
    /* TODO: Handle overlap */
    memCopy(dest, src, count);
}
