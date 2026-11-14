/*
 * TSS services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <status.h>

typedef struct __packed {
    uint16 limitLow;
    uint16 baseLow;
    uint8 baseMid;
    uint16 attributes;
    uint8 baseHigh;
    uint32 baseHigh32;
    uint32 reserved;
} TssDescriptor;

typedef struct __packed {
    uint32 reserved0;
    uint64 rsp0;
    uint64 rsp1;
    uint64 rsp2;
    uint64 reserved1;
    uint64 ist1;
    uint64 ist2;
    uint64 ist3;
    uint64 ist4;
    uint64 ist5;
    uint64 ist6;
    uint64 ist7;
    uint64 reserved2;
    uint16 reserved3;
    uint16 iopb;
} Tss;

Status tssInitialize();
