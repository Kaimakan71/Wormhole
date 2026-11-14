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
#include <cpu/syscall.h>
#include <cpu/tss.h>

void cpuEarlyInitialize() {
    gdtInitialize();
    idtInitialize();
}

Status cpuInitialize() {
    Status status;

    status = tssInitialize();
    if (!STATUS_IS_SUCCESS(status)) {
        return status;
    }

    status = syscallInitialize();
    if (!STATUS_IS_SUCCESS(status)) {
        return status;
    }

    i8259Initialize();
    i8253Initialize();
    i8042Initialize();

    cpuIntEnable();

    return STATUS_SUCCESS;
}
