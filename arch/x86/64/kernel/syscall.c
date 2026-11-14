/*
 * x86-64 syscall handler.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu.h>
#include <cpu/gdt.h>
#include <cpu/msr.h>
#include <cpu/sched.h>
#include <cpu/syscall.h>
#include <log.h>

#define TRACE_SOURCE "syscall"

extern void syscallEntry();

void syscallInitialize() {
    uint64 efer;

    traceDebug("Initializing syscall handler...\n");

    efer = rdmsr(IA32_EFER_MSR);
    SET_BIT(efer, IA32_EFER_SCE);
    wrmsr(IA32_EFER_MSR, efer);

    wrmsr(IA32_STAR_MSR, ((uint64) (USER_DS - 8) << 48) | ((uint64) KERNEL_CS << 32));
    wrmsr(IA32_LSTAR_MSR, (uint64) syscallEntry);
    wrmsr(IA32_FMASK_MSR, FLAGS_IF | FLAGS_DF);
}

CpuThreadState *syscallDispatch(CpuThreadState *state, uintn function) {
    switch (function) {
    case SYSCALL_YIELD:
        state = schedYield(state);
        break;
    default:
        state->rax = STATUS_UNSUPPORTED;
        break;
    }

    return state;
}
