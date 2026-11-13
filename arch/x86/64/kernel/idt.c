/*
 * IDT services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu.h>
#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <cpu/trap.h>

#define IDT_ENTRY_COUNT 256

static __cacheline_aligned IdtEntry idt[IDT_ENTRY_COUNT];

static const Idtr idtr = {
    .limit = sizeof(idt) - 1,
    .base  = (uint64) &idt[0],
};

void idtSetVector(uint8 vector, uint64 offset, uint8 ist, uint8 type, uint8 dpl) {
    IdtEntry *entry;

    entry = &idt[vector];
    entry->offsetLow = offset & 0xffff;
    entry->segment = KERNEL_CS;
    entry->ist = ist;
    entry->reserved0 = 0;
    entry->type = type | IDT_ENTRY_TYPE_32BIT;
    entry->reserved1 = 0;
    entry->dpl = dpl;
    entry->present = 1;
    entry->offsetMid = (offset >> 16) & 0xffff;
    entry->offsetHigh = offset >> 32;
    entry->reserved2 = 0;
}

void idtInitialize() {
    #define INIT_EXCEPTION_ENTRY(vector) idtSetVector(vector, (uint64) isr##vector, 0, IDT_ENTRY_TYPE_TRAP, 0);
    ENUM_EXCEPT_VECTORS(INIT_EXCEPTION_ENTRY);

    ASMV("lidt %0" : : "m" (idtr) : "memory");
}
