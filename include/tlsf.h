/*
 * TLSF allocator services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

#if CONFIG_64BIT == 1
    #define TLSF_ALIGN_LOG2 3
#else
    #define TLSF_ALIGN_LOG2 2
#endif

#define TLSF_ALIGN BIT(TLSF_ALIGN_LOG2)

void *tlsfInitialize(void *memory, uintn size);
void *tlsfAllocate(void *pool, uintn size);
void tlsfFree(void *pool, void *ptr);
void *tlsfResize(void *pool, void *ptr, uintn size);
