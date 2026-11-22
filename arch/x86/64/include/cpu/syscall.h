/*
 * Syscall services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#define SYSCALL_THREAD_EXIT 0

static inline Status syscall0(uintn function) {
    Status status;
    ASMV("syscall" : "=a" (status) : "a" (function) : "rcx", "r11", "memory");
    return status;
}

void syscallInitialize();
