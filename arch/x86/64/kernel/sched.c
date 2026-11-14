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

static void schedSetCurrentThread(Thread *thread) {
    cpuGlobalState.currentThread = thread;
    cpuGlobalState.kernelStackTop = thread->kernelStackTop;
}

Status schedStart() {
    Thread *nextThread;

    traceDebug("Starting scheduler...\n");

    nextThread = schedSchedule(NULL);
    if (nextThread == NULL) {
        return STATUS_UNKNOWN_ERROR;
    }

    schedSetCurrentThread(nextThread);
    syscall0(SYSCALL_YIELD);
    unreachable();
}

CpuThreadState *schedYield() {
    Thread *currentThread, *nextThread;

    currentThread = cpuGlobalState.currentThread;
    nextThread = schedSchedule(currentThread);
    if (nextThread != currentThread) {
        schedSetCurrentThread(nextThread);
    }

    return &nextThread->cpuState;
}
