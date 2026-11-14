/*
 * x86-64 scheduler services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu.h>
#include <cpu/sched.h>
#include <cpu/syscall.h>
#include <log.h>
#include <sched.h>

#define TRACE_SOURCE "sched"

void schedStart() {
    traceDebug("Starting scheduler...\n");
    ASMV("syscall" : : "a" (SYSCALL_YIELD) : "rcx", "r11", "memory");
}

void schedYield(CpuState *regs) {
    (void) regs;

    traceDebug("Yield!\n");

    for (;;) {
        ASMV("pause");
    }
}
