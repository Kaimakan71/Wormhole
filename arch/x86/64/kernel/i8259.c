/*
 * i8259 interrupt controller driver.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu/idt.h>
#include <cpu/i8259.h>
#include <cpu/pmio.h>
#include <log.h>

#define TRACE_SOURCE "i8259"

void i8259Initialize() {
    traceDebug("Initializing PIC...\n");

    /* Begin initialization */
    pmioWrite8(I8259_MASTER_COMMAND_PORT, I8259_ICW1_INITIALIZE | I8259_ICW1_ICW4);
    pmioWrite8(I8259_SLAVE_COMMAND_PORT, I8259_ICW1_INITIALIZE | I8259_ICW1_ICW4);

    /* Set vector offset */
    pmioWrite8(I8259_MASTER_DATA_PORT, IDT_IRQ_OFFSET);
    pmioWrite8(I8259_SLAVE_DATA_PORT, IDT_IRQ_OFFSET + I8259_SLAVE_OFFSET);

    /* Set cascade IRQ */
    pmioWrite8(I8259_MASTER_DATA_PORT, BIT(I8259_CASCADE_IRQ));
    pmioWrite8(I8259_SLAVE_DATA_PORT, I8259_CASCADE_IRQ);

    /* Set 8086 mode */
    pmioWrite8(I8259_MASTER_DATA_PORT, I8259_ICW4_8086);
    pmioWrite8(I8259_SLAVE_DATA_PORT, I8259_ICW4_8086);

    /* Mask all interrupts */
    pmioWrite8(I8259_MASTER_DATA_PORT, 0xff);
    pmioWrite8(I8259_SLAVE_DATA_PORT, 0xff);
}

void i8259Unmask(uint8 irq) {
    if (irq < I8259_SLAVE_OFFSET) {
        pmioWrite8(I8259_MASTER_DATA_PORT, pmioRead8(I8259_MASTER_DATA_PORT) & ~BIT(irq));
    } else {
        pmioWrite8(I8259_SLAVE_DATA_PORT, pmioRead8(I8259_SLAVE_DATA_PORT) & ~BIT(irq - I8259_SLAVE_OFFSET));
    }
}

void i8259Eoi(uint8 irq) {
    if (irq >= I8259_SLAVE_OFFSET) {
        pmioWrite8(I8259_SLAVE_COMMAND_PORT, I8259_COMMAND_EOI);
    }

    pmioWrite8(I8259_MASTER_COMMAND_PORT, I8259_COMMAND_EOI);
}
