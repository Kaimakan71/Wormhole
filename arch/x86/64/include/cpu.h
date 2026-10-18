/*
 * x86-64 CPU services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

static inline void cpuSetCr3(uint64 cr3) {
    ASMV("movq %0, %%cr3" : : "r" (cr3) : "memory");
}
