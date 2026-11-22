/*
 * x86-64 thread management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu/gdt.h>
#include <cpu/state.h>
#include <cpu/tss.h>
#include <thread.h>

extern __noreturn void threadArchSwitchToUser(CpuThreadState *cpuState);
extern __noreturn void threadArchSwitchToKernel(CpuThreadState *cpuState);

void threadArchInitialize(Thread *thread, void *entry) {
    CpuThreadState *state;

    state = (CpuThreadState *) (thread->kernelStackTop - sizeof(*state));
    state->cs = USER_CS;
    state->rip = (uint64) entry;
    state->ss = USER_DS;
    state->rsp = (uint64) thread->stack + thread->stackSize;
    thread->cpuState = state;
}

/* TODO: Make make these inline */

Thread *threadArchGetCurrent() {
    return cpuGlobalState.currentThread;
}

void threadArchSetCurrent(Thread *thread) {
    cpuGlobalState.currentThread = thread;
    cpuGlobalState.kernelStackTop = thread->kernelStackTop;
    tss.rsp0 = thread->kernelStackTop;
}

__noreturn void threadArchSwitchTo(Thread *thread) {
    threadArchSetCurrent(thread);

    if (thread->flags & THREAD_FLAG_KERNEL) {
        threadArchSwitchToKernel(thread->cpuState);
    } else {
        threadArchSwitchToUser(thread->cpuState);
    }
}
