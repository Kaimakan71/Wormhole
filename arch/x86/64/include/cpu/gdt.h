/*
 * GDT services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

#define KERNEL_CS 0x08
#define KERNEL_DS 0x10
#define USER_CS   0x18
#define USER_DS   0x20

#define GDT_ENTRY_ATTRIBUTE_ACCESSED    BIT(0)
#define GDT_ENTRY_ATTRIBUTE_EXECUTABLE  BIT(3)
#define GDT_ENTRY_ATTRIBUTE_SYSTEM      BIT(4)
#define GDT_ENTRY_ATTRIBUTE_PRESENT     BIT(7)
#define GDT_ENTRY_ATTRIBUTE_AVAILABLE   BIT(12)
#define GDT_ENTRY_ATTRIBUTE_64BIT_CODE  BIT(13)
#define GDT_ENTRY_ATTRIBUTE_32BIT       BIT(14)
#define GDT_ENTRY_ATTRIBUTE_GRANULARITY BIT(15)

/* Code segments only */
#define GDT_ENTRY_ATTRIBUTE_READABLE   BIT(1)
#define GDT_ENTRY_ATTRIBUTE_CONFORMING BIT(2)

/* Data segments only */
#define GDT_ENTRY_ATTRIBUTE_WRITABLE    BIT(1)
#define GDT_ENTRY_ATTRIBUTE_EXPAND_DOWN BIT(2)

typedef struct __packed {
    uint16 limitLow;
    uint16 baseLow;
    uint8 baseMid;
    uint16 attributes;
    uint8 baseHigh;
} GdtEntry;

typedef struct __packed {
    uint16 limit;
    uint64 base;
} Gdtr;

void gdtInitialize();
