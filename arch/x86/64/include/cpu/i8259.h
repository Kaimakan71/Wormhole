/*
 * i8259 interrupt controller driver.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

#define I8259_MASTER_COMMAND_PORT 0x20
#define I8259_MASTER_DATA_PORT    0x21
#define I8259_SLAVE_COMMAND_PORT  0xa0
#define I8259_SLAVE_DATA_PORT     0xa1

#define I8259_COMMAND_EOI 0x20

#define I8259_ICW1_ICW4       BIT(0)
#define I8259_ICW1_CASCADE    BIT(1)
#define I8259_ICW1_INTERVAL_4 BIT(2)
#define I8259_ICW1_LEVEL      BIT(3)
#define I8259_ICW1_INITIALIZE BIT(4)

#define I8259_ICW4_8086       BIT(0)
#define I8259_ICW4_AUTO_EOI   BIT(1)
#define I8259_ICW4_BUF_SLAVE  BIT(2)
#define I8259_ICW4_BUF_MASTER BIT(3)
#define I8259_ICW4_SFNM       BIT(4)

#define I8259_CASCADE_IRQ 2

#define I8259_VECTOR_OFFSET 0x20

#define I8259_SLAVE_OFFSET 8

void i8259Initialize();
void i8259Unmask(uint8 irq);
void i8259Eoi(uint8 irq);
