/*
 * x86-64 scheduler services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu.h>
#include <cpu/sched.h>
#include <cpu/state.h>
#include <cpu/syscall.h>
#include <log.h>
#include <sched.h>
#include <thread.h>

#define TRACE_SOURCE "sched"

#define syscall0(function) ASMV("syscall" : : "a" (function) : "rcx", "r11", "memory")

__noreturn void schedStart() {
    Thread *idleThread;

    traceDebug("Starting scheduler...\n");

    idleThread = (Thread *) schedIdleProcess->threads.first;
    cpuGlobalState.currentThread = idleThread;
    cpuGlobalState.kernelStackTop = idleThread->kernelStackTop;
    syscall0(SYSCALL_YIELD);

    traceDebug("Idling...\n");
    for (;;) {
        cpuPause();
    }
}

CpuThreadState *schedYield(CpuThreadState *state) {
    Thread *currentThread;

    currentThread = cpuGlobalState.currentThread;

    traceDebug("TID %u yielding...\n", currentThread->id);

    /* TODO: Schedule new thread */

    return state;
}
