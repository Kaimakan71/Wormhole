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

#define IDT_ENTRY(_offset, _segment, _ist, _type, _dpl) { \
    .offsetLow  = (_offset) & 0xffff,                     \
    .segment    = (_segment),                             \
    .ist        = (_ist),                                 \
    .reserved0  = 0,                                      \
    .type       = (_type),                                \
    .reserved1  = 0,                                      \
    .dpl        = (_dpl),                                 \
    .present    = 1,                                      \
    .offsetMid  = ((_offset) >> 16) & 0xffff,             \
    .offsetHigh = (_offset) >> 32,                        \
    .reserved2  = 0,                                      \
}

static __cacheline_aligned IdtEntry idt[IDT_ENTRY_COUNT] = {
    #define MAKE_EXCEPTION_ENTRY(vector) IDT_ENTRY(0ULL, KERNEL_CS, 0, IDT_ENTRY_TYPE_32BIT | IDT_ENTRY_TYPE_TRAP, 0),
    ENUM_EXCEPT_VECTORS(MAKE_EXCEPTION_ENTRY)
};

static const Idtr idtr = {
    .limit = sizeof(idt) - 1,
    .base  = (uint64) &idt[0],
};

void idtInitialize() {
    #define INIT_EXCEPTION_ENTRY(vector) {                               \
        idt[vector].offsetLow  = (uint64) (isr##vector) & 0xffff;        \
        idt[vector].offsetMid = ((uint64) (isr##vector) >> 16) & 0xffff; \
        idt[vector].offsetHigh  = (uint64) (isr##vector) >> 32;          \
    }
    ENUM_EXCEPT_VECTORS(INIT_EXCEPTION_ENTRY);

    ASMV("lidt %0" : : "m" (idtr) : "memory");
}
