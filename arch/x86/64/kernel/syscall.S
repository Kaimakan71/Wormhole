/*
 * x86-64 syscall entry.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <asm.h>

.extern syscallDispatch

BEGIN_FUNC(syscallEntry)
    swapgs

    mov %rsp, %gs:8
    mov %gs:0, %rsp

    pushq $0x1b   /* User SS */
    pushq %gs:8   /* User RSP */
    push %r11     /* User RFLAGS */
    pushq $0x23   /* User CS */
    push %rcx     /* User RIP */
    sub $16, %rsp /* Error code and vector */

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

    sti

    mov %rsp, %rdi
    call syscallDispatch

    cli

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

    mov %gs:8, %rsp

    swapgs
    sysretq
END_FUNC(syscallEntry)
