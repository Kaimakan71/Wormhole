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

void cpuEarlyInitialize() {
    gdtInitialize();
    idtInitialize();
}

void cpuInitialize() {
    i8259Initialize();
    i8253Initialize();
    i8042Initialize();
    cpuIntEnable();
}
