/*
 * x86-64 thread management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu/gdt.h>
#include <thread.h>

void threadArchInitialize(Thread *thread, void *entry) {
    CpuThreadState *state;

    state = &thread->cpuState;
    state->cs = USER_CS;
    state->rip = (uint64) entry;
    state->ss = USER_DS;
    state->rsp = (uint64) thread->stack + thread->stackSize;
}
