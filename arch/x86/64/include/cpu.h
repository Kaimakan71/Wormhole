/*
 * x86-64 CPU services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <status.h>

#define CACHE_LINE_SIZE 64

#define __cacheline_aligned __align(CACHE_LINE_SIZE)

#define cpuIntEnable()  ASMV("sti")
#define cpuIntDisable() ASMV("cli")
#define cpuPause()      ASMV("pause")
#define cpuHalt()       ASMV("hlt")

#define cpuHang() ({  \
    ASMV("cli; hlt"); \
    unreachable();    \
})

#define FLAGS_IF BIT(9)
#define FLAGS_DF BIT(10)

static inline uint64 cpuGetRip() {
    uint64 rip;

    ASMV("lea (%%rip), %0" : "=r" (rip));

    return rip;
}

static inline uint64 cpuGetRsp() {
    uint64 rsp;

    ASMV("mov %%rsp, %0" : "=r" (rsp));

    return rsp;
}

static inline void cpuSetCr3(uint64 cr3) {
    ASMV("movq %0, %%cr3" : : "r" (cr3) : "memory");
}

void cpuEarlyInitialize();
Status cpuInitialize();
