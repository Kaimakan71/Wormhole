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
    outb(I8259_MASTER_COMMAND_PORT, I8259_ICW1_INITIALIZE | I8259_ICW1_ICW4);
    outb(I8259_SLAVE_COMMAND_PORT, I8259_ICW1_INITIALIZE | I8259_ICW1_ICW4);

    /* Set vector offset */
    outb(I8259_MASTER_DATA_PORT, IDT_IRQ_OFFSET);
    outb(I8259_SLAVE_DATA_PORT, IDT_IRQ_OFFSET + I8259_SLAVE_OFFSET);

    /* Set cascade IRQ */
    outb(I8259_MASTER_DATA_PORT, BIT(I8259_CASCADE_IRQ));
    outb(I8259_SLAVE_DATA_PORT, I8259_CASCADE_IRQ);

    /* Set 8086 mode */
    outb(I8259_MASTER_DATA_PORT, I8259_ICW4_8086);
    outb(I8259_SLAVE_DATA_PORT, I8259_ICW4_8086);

    /* Mask all interrupts */
    outb(I8259_MASTER_DATA_PORT, 0xff);
    outb(I8259_SLAVE_DATA_PORT, 0xff);
}

void i8259Unmask(uint8 irq) {
    if (irq < I8259_SLAVE_OFFSET) {
        outb(I8259_MASTER_DATA_PORT, inb(I8259_MASTER_DATA_PORT) & ~BIT(irq));
    } else {
        outb(I8259_SLAVE_DATA_PORT, inb(I8259_SLAVE_DATA_PORT) & ~BIT(irq - I8259_SLAVE_OFFSET));
    }
}

void i8259Eoi(uint8 irq) {
    if (irq >= I8259_SLAVE_OFFSET) {
        outb(I8259_SLAVE_COMMAND_PORT, I8259_COMMAND_EOI);
    }

    outb(I8259_MASTER_COMMAND_PORT, I8259_COMMAND_EOI);
}
