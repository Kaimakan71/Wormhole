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
#include <mm.h>

#define TRACE_SOURCE "syscall"

#define STACK_SIZE 0x1000

extern void syscallEntry();

static struct {
    uint64 kernelRsp;
    uint64 userRsp;
} stacks;

Status syscallInitialize() {
    uintn pageCount;
    Status status;
    void *stack;
    uint64 efer;

    traceDebug("Initializing syscall handler...\n");

    pageCount = SIZE_TO_PAGES(STACK_SIZE);
    status = vmmAllocatePages(pageCount, &stack);
    if (!STATUS_IS_SUCCESS(status)) {
        traceError("Failed to allocate %u page(s) for syscall stack\n", pageCount);
        return status;
    }

    stacks.userRsp = 0;
    stacks.kernelRsp = (uint64) stack + STACK_SIZE;

    efer = rdmsr(IA32_EFER_MSR);
    SET_BIT(efer, IA32_EFER_SCE);
    wrmsr(IA32_EFER_MSR, efer);

    wrmsr(IA32_STAR_MSR, ((uint64) (USER_DS - 8) << 48) | ((uint64) KERNEL_CS << 32));
    wrmsr(IA32_LSTAR_MSR, (uint64) syscallEntry);
    wrmsr(IA32_FMASK_MSR, FLAGS_IF | FLAGS_DF);
    wrmsr(IA32_KERNEL_GS_BASE_MSR, (uint64) &stacks);

    return STATUS_SUCCESS;
}

void syscallDispatch(CpuState *regs) {
    if (regs->rax == SYSCALL_YIELD) {
        schedYield(regs);
    }
}
