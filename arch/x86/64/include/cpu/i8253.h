/*
 * i8253 interval timer driver.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#define I8253_CH0_DATA_PORT 0x40
#define I8253_CH1_DATA_PORT 0x41
#define I8253_CH2_DATA_PORT 0x42
#define I8253_COMMAND_PORT  0x43

#define I8253_TIMER_IRQ 0

void i8253Initialize();
