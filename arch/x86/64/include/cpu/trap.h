/*
 * Trap handling services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#define ENUM_EXCEPT_VECTORS(macro) \
    macro(0)                       \
    macro(1)                       \
    macro(2)                       \
    macro(3)                       \
    macro(4)                       \
    macro(5)                       \
    macro(6)                       \
    macro(7)                       \
    macro(8)                       \
    macro(9)                       \
    macro(10)                      \
    macro(11)                      \
    macro(12)                      \
    macro(13)                      \
    macro(14)                      \
    macro(15)                      \
    macro(16)                      \
    macro(17)                      \
    macro(18)                      \
    macro(19)                      \
    macro(20)                      \
    macro(21)

#ifndef __ASSEMBLER__

#include <types.h>

typedef struct __packed {
    /* Pushed by kernel ISR */
    uint64 r15;
    uint64 r14;
    uint64 r13;
    uint64 r12;
    uint64 r11;
    uint64 r10;
    uint64 r9;
    uint64 r8;
    uint64 rbp;
    uint64 rdi;
    uint64 rsi;
    uint64 rdx;
    uint64 rcx;
    uint64 rbx;
    uint64 rax;
    uint64 vector;

    /* Pushed by CPU */
    uint64 errorCode;
    uint64 rip;
    uint64 cs;
    uint64 rflags;
    uint64 rsp;
    uint64 ss;
} Trapframe;

void trapException(Trapframe *tf);

#define DECLARE_ISR(vector) extern void isr##vector(void);
ENUM_EXCEPT_VECTORS(DECLARE_ISR);

#endif
