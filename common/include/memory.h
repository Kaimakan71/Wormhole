/*
 * Memory manipulation services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

static inline void memFill(void *destination, uint8 value, uintn count) {
    uint8 *d;

    d = destination;
    while (count--) {
        *d++ = value;
    }
}

static inline void memClear(void *destination, uintn count) {
    memFill(destination, 0, count);
}

static inline void memCopy(void *destination, const void *source, uintn count) {
    uint8 *d;
    const uint8 *s;

    d = destination;
    s = source;
    while (count--) {
        *d++ = *s++;
    }
}

static inline void memMove(void *destination, const void *source, uintn count) {
    uint8 *d;
    const uint8 *s;

    if (destination == source || count == 0) {
        return;
    }

    d = destination;
    s = source;
    if (d < s) {
        while (count--) {
            *d++ = *s++;
        }
    } else {
        d += count;
        s += count;
        while (count--) {
            *--d = *--s;
        }
    }
}
