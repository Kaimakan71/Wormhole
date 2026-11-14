/*
 * x86-64 syscall entry.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <asm.h>
#include <cpu/gdt.h>

.extern syscallDispatch

BEGIN_FUNC(syscallEntry)
    swapgs

    /* Switch to thread's kernel stack */
    mov %rsp, %gs:8
    mov %gs:0, %rsp

    pushq $USER_DS /* User SS */
    pushq %gs:8    /* User RSP */
    push %r11      /* User RFLAGS */
    pushq $USER_CS /* User CS */
    push %rcx      /* User RIP */
    push %rax      /* Error code */
    push %rax      /* Vector */

    /* Save thread state */
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

    /* Dispatch syscall */
    mov %rsp, %rdi
    mov %rax, %rsi
    call syscallDispatch

    /* Switch to new kernel stack */
    mov %rax, %rsp

    /* Restore new thread state */
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

    pop %rcx /* Ignore vector */
    pop %rcx /* Ignore error code */
    pop %rcx /* User RIP */
    pop %r11 /* Ignore CS */
    pop %r11 /* User RFLAGS */

    /* Switch to new user stack */
    mov %gs:8, %rsp

    swapgs
    sysretq
END_FUNC(syscallEntry)
