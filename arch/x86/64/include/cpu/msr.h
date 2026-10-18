/*
 * MSR services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

#define IA32_APIC_BASE_MSR    0x0000001b
#define IA32_APIC_BASE_BSP    BIT(8)
#define IA32_APIC_BASE_ENABLE BIT(11)

static inline uint64 rdmsr(uint32 msr) {
    uint32 lo, hi;

    ASMV("rdmsr" : "=a" (lo), "=d" (hi) : "c" (msr));

    return ((uint64) hi << 32) | lo;
}

static inline void wrmsr(uint32 msr, uint64 value) {
    uint32 lo, hi;

    lo = (uint32) value;
    hi = (uint32) (value >> 32);

    ASMV("wrmsr" : : "c" (msr), "a" (lo), "d" (hi) : "memory");
}
