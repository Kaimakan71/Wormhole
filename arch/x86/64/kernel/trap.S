/*
 * Trap handling services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <asm.h>
#include <cpu/trap.h>

.extern trapException

.macro tfPush vector
    .if \vector != 8 && \vector != 10 && \vector != 11 && \vector != 12 && \vector != 13 && \vector != 14
        pushq $0
    .endif

    pushq $\vector
    push %rax
    push %rbx
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %rbp
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15
.endm

.macro tfPop vector
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %r11
    pop %r10
    pop %r9
    pop %r8
    pop %rbp
    pop %rdi
    pop %rsi
    pop %rdx
    pop %rcx
    pop %rbx
    pop %rax

    .if \vector != 8 && \vector != 10 && \vector != 11 && \vector != 12 && \vector != 13 && \vector != 14
        add $16, %rsp
    .else
        add $8, %rsp
    .endif
.endm

#define MAKE_EXCEPT_ISR(vector) \
.align 16;                      \
BEGIN_FUNC(isr##vector);        \
    cli;                        \
    tfPush vector;              \
    mov %rsp, %rdi;             \
    call trapException;         \
    tfPop vector;               \
    sti;                        \
    iretq;                      \
END_FUNC(isr##vector);

ENUM_EXCEPT_VECTORS(MAKE_EXCEPT_ISR)
