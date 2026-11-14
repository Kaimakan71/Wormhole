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

#define syscall0(function) ASMV("syscall" : : "a" (function) : "rcx", "r11", "memory")

void schedStart() {
    traceDebug("Starting scheduler...\n");
    syscall0(SYSCALL_YIELD);

    traceDebug("Idling...\n");
    for (;;) {
        cpuPause();
    }
}

CpuThreadState *schedYield(CpuThreadState *state) {
    traceDebug("Thread requested yield\n");

    /* TODO: Schedule new thread here */

    return state;
}
