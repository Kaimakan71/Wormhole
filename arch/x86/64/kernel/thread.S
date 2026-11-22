/*
 * x86-64 thread management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <asm.h>

.macro switchRegs
    /* Switch to new kernel stack */
    mov %rdi, %rsp

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
.endm

.align 16
BEGIN_FUNC(threadArchSwitchToUser)
    switchRegs

    pop %rcx /* Vector */
    pop %rcx /* Error code */
    pop %rcx /* User RIP */
    pop %r11 /* User CS */
    pop %r11 /* User RFLAGS */
    pop %rsp /* User RSP */

    swapgs
    sysretq
END_FUNC(threadArchSwitchToUser)

.align 16
BEGIN_FUNC(threadArchSwitchToKernel)
    switchRegs

    pop %rcx /* Vector */
    pop %rcx /* Error code */
    pop %rcx /* Kernel RIP */
    pop %r11 /* Kernel CS */
    popfq    /* Kernel RFLAGS */
    pop %rsp /* Kernel RSP */

    jmp *%rcx
END_FUNC(threadArchSwitchToKernel)
