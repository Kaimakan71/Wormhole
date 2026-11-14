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

#define DECLARE_ISR(vector) extern void isr##vector();
ENUM_EXCEPT_VECTORS(DECLARE_ISR);

#endif
