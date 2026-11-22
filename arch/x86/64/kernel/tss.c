/*
 * TSS services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu/gdt.h>
#include <cpu/tss.h>
#include <log.h>
#include <memory.h>

#define TRACE_SOURCE "tss"

__cacheline_aligned Tss tss;

void tssInitialize() {
    uint64 base;
    TssDescriptor *desc;

    traceDebug("Initializing TSS...\n");

    memClear(&tss, sizeof(tss));

    base = (uint64) &tss;
    desc = (TssDescriptor *) &gdt[GDT_TSS_ENTRY];
    desc->limitLow = sizeof(tss) - 1;
    desc->baseLow = base & 0xffff;
    desc->baseMid = (base >> 16) & 0xff;
    desc->attributes = GDT_ENTRY_ATTRIBUTE_PRESENT  | GDT_ENTRY_ATTRIBUTE_EXECUTABLE |
                       GDT_ENTRY_ATTRIBUTE_ACCESSED | GDT_ENTRY_DPL(0);
    desc->baseHigh = (base >> 24) & 0xff;
    desc->baseHigh32 = (base >> 32) & 0xffffffff;
    desc->reserved = 0;

    ASMV("ltr %0" : : "rm" (GDT_TSS_SELECTOR) : "memory");
}
