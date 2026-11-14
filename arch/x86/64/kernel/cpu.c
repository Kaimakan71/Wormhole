/*
 * x86-64 CPU services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu.h>
#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <cpu/i8042.h>
#include <cpu/i8253.h>
#include <cpu/i8259.h>
#include <cpu/msr.h>
#include <cpu/state.h>
#include <cpu/syscall.h>
#include <cpu/tss.h>
#include <log.h>
#include <mm.h>

#define TRACE_SOURCE "cpu"

__cacheline_aligned CpuGlobalState cpuGlobalState;

void cpuEarlyInitialize() {
    gdtInitialize();
    idtInitialize();
}

Status cpuInitialize() {
    Status status;
    void *stack;

    traceDebug("Initializing CPU...\n");

    status = tssInitialize();
    if (!STATUS_IS_SUCCESS(status)) {
        return status;
    }

    /* TODO: Allocate stack per-thread */
    status = vmmAllocatePages(1, &stack);
    if (!STATUS_IS_SUCCESS(status)) {
        traceError("Failed to allocate page for kernel stack\n");
        return status;
    }
    cpuGlobalState.kernelStackTop = (uint64) stack + PAGE_SIZE;

    wrmsr(IA32_KERNEL_GS_BASE_MSR, (uint64) &cpuGlobalState);
    syscallInitialize();
    i8259Initialize();
    i8253Initialize();
    i8042Initialize();

    cpuIntEnable();

    return STATUS_SUCCESS;
}
