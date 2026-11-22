/*
 * x86-64 syscall handler.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu.h>
#include <cpu/gdt.h>
#include <cpu/msr.h>
#include <cpu/state.h>
#include <cpu/syscall.h>
#include <log.h>

#define TRACE_SOURCE "syscall"

extern void syscallEntry();

static CpuThreadState *sysThreadExit(Thread *thread, CpuThreadState *threadState) {
    (void) thread;
    (void) threadState;

    /* TODO: Implement this routine */
    traceDebug("TODO: Implement sysThreadExit()\n");

    cpuHang();
}

static __cacheline_aligned CpuThreadState *(*syscallTable[])(Thread *, CpuThreadState *) = {
    [SYSCALL_THREAD_EXIT]  = sysThreadExit,
};

void syscallInitialize() {
    uint64 efer;

    traceDebug("Initializing syscall handler...\n");

    /* Configure dispatch process */
    wrmsr(IA32_STAR_MSR, ((uint64) (USER_DS - 8) << 48) | ((uint64) KERNEL_CS << 32));
    wrmsr(IA32_LSTAR_MSR, (uint64) syscallEntry);
    wrmsr(IA32_FMASK_MSR, FLAGS_IF | FLAGS_DF);

    /* Enable syscall instruction */
    efer = rdmsr(IA32_EFER_MSR);
    SET_BIT(efer, IA32_EFER_SCE);
    wrmsr(IA32_EFER_MSR, efer);
}

CpuThreadState *syscallDispatch(CpuThreadState *state, uintn function) {
    if (function < ARRAY_LENGTH(syscallTable)) {
        state = syscallTable[function](cpuGlobalState.currentThread, state);
    } else {
        state->rax = STATUS_UNSUPPORTED;
    }

    return state;
}
