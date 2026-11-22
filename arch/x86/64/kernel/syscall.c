/*
 * x86-64 syscall handler.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu/gdt.h>
#include <cpu/msr.h>
#include <cpu/state.h>
#include <cpu/syscall.h>
#include <log.h>
#include <sched.h>

#define TRACE_SOURCE "syscall"

extern void syscallEntry();

static __cacheline_aligned Status (*syscallTable[])(Thread *) = {
    [SYSCALL_THREAD_EXIT] = schedThreadExit,
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

void syscallDispatch(CpuThreadState *state, uintn function) {
    Status status;

    if (function < ARRAY_LENGTH(syscallTable)) {
        status = syscallTable[function](cpuGlobalState.currentThread);
    } else {
        status = STATUS_UNSUPPORTED;
    }

    state->rax = status;
}
