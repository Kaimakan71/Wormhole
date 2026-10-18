/*
 * IDT services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

#define IDT_ENTRY_TYPE_TASK      5
#define IDT_ENTRY_TYPE_INTERRUPT 6
#define IDT_ENTRY_TYPE_TRAP      7
#define IDT_ENTRY_TYPE_32BIT     BIT(3)

typedef struct __packed {
    uint16 offsetLow;
    uint16 segment;
    uint8 ist: 3;
    uint8 reserved0: 5;
    uint8 type: 4;
    uint8 reserved1: 1;
    uint8 dpl: 2;
    uint8 present: 1;
    uint16 offsetMid;
    uint32 offsetHigh;
    uint32 reserved2;
} IdtEntry;

typedef struct __packed {
    uint16 limit;
    uint64 base;
} Idtr;

void idtInitialize();
