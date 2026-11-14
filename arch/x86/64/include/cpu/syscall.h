/*
 * Syscall services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

typedef struct __packed {
    uint64 r15;
    uint64 r14;
    uint64 r13;
    uint64 r12;
    uint64 rflags;
    uint64 r10;
    uint64 r9;
    uint64 r8;
    uint64 rbp;
    uint64 rdi;
    uint64 rsi;
    uint64 rdx;
    uint64 rip;
    uint64 rbx;
    uint64 rax;
    uint64 rsp;
} SyscallRegs;

Status syscallInitialize();
