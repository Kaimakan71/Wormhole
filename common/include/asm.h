/*
 * Common assembler definitions.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#ifdef __ELF__

#define BEGIN_FUNC(name)    \
    .globl name ;           \
    .type name, %function ; \
name:

#define END_FUNC(name) \
    .size name, . - name

#else

#define BEGIN_FUNC(name) \
    .globl name ;        \
name:

#define END_FUNC(name)

#endif
