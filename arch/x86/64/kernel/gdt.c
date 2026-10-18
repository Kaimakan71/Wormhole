/*
 * GDT services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu.h>
#include <cpu/gdt.h>

#define GDT_ENTRY_COUNT 5

static __cacheline_aligned const GdtEntry gdt[GDT_ENTRY_COUNT] = {
    /* Reserved by CPU */
    {
        .limitLow   = 0x0000,
        .baseLow    = 0x0000,
        .baseMid    = 0x00,
        .attributes = 0x0000,
        .baseHigh   = 0x00,
    },

    /* Kernel code */
    {
        .limitLow   = 0xffff,
        .baseLow    = 0x0000,
        .baseMid    = 0x00,
        .attributes = GDT_ENTRY_ATTRIBUTE_GRANULARITY | GDT_ENTRY_ATTRIBUTE_64BIT_CODE |
                      GDT_ENTRY_ATTRIBUTE_PRESENT     | GDT_ENTRY_ATTRIBUTE_SYSTEM     |
                      GDT_ENTRY_ATTRIBUTE_EXECUTABLE  | GDT_ENTRY_ATTRIBUTE_READABLE,
        .baseHigh   = 0x00,
    },

    /* Kernel data */
    {
        .limitLow  = 0xffff,
        .baseLow   = 0x0000,
        .baseMid   = 0x00,
        .attributes = GDT_ENTRY_ATTRIBUTE_GRANULARITY | GDT_ENTRY_ATTRIBUTE_PRESENT |
                      GDT_ENTRY_ATTRIBUTE_SYSTEM      | GDT_ENTRY_ATTRIBUTE_WRITABLE,
        .baseHigh  = 0x00,
    },

    /* User code */
    {
        .limitLow   = 0xffff,
        .baseLow    = 0x0000,
        .baseMid    = 0x00,
        .attributes = GDT_ENTRY_ATTRIBUTE_GRANULARITY | GDT_ENTRY_ATTRIBUTE_64BIT_CODE |
                      GDT_ENTRY_ATTRIBUTE_PRESENT     | GDT_ENTRY_ATTRIBUTE_SYSTEM     |
                      GDT_ENTRY_ATTRIBUTE_EXECUTABLE  | GDT_ENTRY_ATTRIBUTE_READABLE,
        .baseHigh   = 0x00,
    },

    /* User data */
    {
        .limitLow  = 0xffff,
        .baseLow   = 0x0000,
        .baseMid   = 0x00,
        .attributes = GDT_ENTRY_ATTRIBUTE_GRANULARITY | GDT_ENTRY_ATTRIBUTE_PRESENT |
                      GDT_ENTRY_ATTRIBUTE_SYSTEM      | GDT_ENTRY_ATTRIBUTE_WRITABLE,
        .baseHigh  = 0x00,
    },
};

static const Gdtr gdtr = {
    .limit = sizeof(gdt) - 1,
    .base  = (uint64) &gdt[0],
};

void gdtInitialize() {
    ASMV(
        "lgdt %0\n"
        "push %1\n"
        "lea 1f(%%rip), %%rax\n"
        "push %%rax\n"
        "lretq\n"
        "1:\n"
        "\tmov %2, %%ax\n"
        "\tmov %%ax, %%ds\n"
        "\tmov %%ax, %%es\n"
        "\tmov %%ax, %%fs\n"
        "\tmov %%ax, %%gs\n"
        "\tmov %%ax, %%ss\n"
        :
        : "m" (gdtr), "i" (KERNEL_CS), "i" (KERNEL_DS)
        : "rax", "memory"
    );
}
