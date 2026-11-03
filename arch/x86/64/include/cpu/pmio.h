/*
 * Port-mapped I/O services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

static inline uint8 pmioRead8(uint16 port) {
    uint8 data;
    ASMV("inb %1, %0" : "=a" (data) : "Nd" (port));
    return data;
}

static inline uint16 pmioRead16(uint16 port) {
    uint16 data;
    ASMV("inw %1, %0" : "=a" (data) : "Nd" (port));
    return data;
}

static inline uint32 pmioRead32(uint16 port) {
    uint32 data;
    ASMV("inl %1, %0" : "=a" (data) : "Nd" (port));
    return data;
}

static inline void pmioWrite8(uint16 port, uint8 data) {
    ASMV("outb %0, %1" : : "a" (data), "Nd" (port));
}

static inline void pmioWrite16(uint16 port, uint16 data) {
    ASMV("outw %0, %1" : : "a" (data), "Nd" (port));
}

static inline void pmioWrite32(uint16 port, uint32 data) {
    ASMV("outl %0, %1" : : "a" (data), "Nd" (port));
}
