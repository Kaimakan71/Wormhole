/*
 * Port I/O services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

static inline uint8 inb(uint16 port) {
    uint8 data;

    ASMV("inb %1, %0" : "=a" (data) : "Nd" (port));

    return data;
}

static inline void outb(uint16 port, uint8 data) {
    ASMV("outb %0, %1" : : "a" (data), "Nd" (port));
}
