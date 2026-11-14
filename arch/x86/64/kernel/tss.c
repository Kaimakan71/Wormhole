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
#include <mm.h>

#define TRACE_SOURCE "tss"

#define STACK_SIZE 0x1000

static Tss tss;

Status tssInitialize() {
    uintn pageCount;
    Status status;
    void *stack;
    uint64 base;
    TssDescriptor *desc;

    traceDebug("Initializing TSS...\n");

    pageCount = SIZE_TO_PAGES(STACK_SIZE);
    status = vmmAllocatePages(pageCount, &stack);
    if (!STATUS_IS_SUCCESS(status)) {
        traceError("Failed to allocate %u page(s) for TSS stack\n", pageCount);
        return status;
    }

    memClear(&tss, sizeof(tss));
    tss.rsp0 = (uint64) stack;

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

    return STATUS_SUCCESS;
}
