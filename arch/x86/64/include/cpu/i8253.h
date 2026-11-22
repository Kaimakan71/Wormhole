/*
 * i8253 interval timer driver.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <device.h>

#define I8253_CH0_DATA_PORT 0x40
#define I8253_CH1_DATA_PORT 0x41
#define I8253_CH2_DATA_PORT 0x42
#define I8253_COMMAND_PORT  0x43

#define I8253_CHANNEL(ch) ((ch) << 6)

#define I8253_ACCESS_LATCH   0x00
#define I8253_ACCESS_LO_ONLY 0x10
#define I8253_ACCESS_HI_ONLY 0x20
#define I8253_ACCESS_LOHI    0x30

#define I8253_MODE_IOTC        0x00
#define I8253_MODE_SQUARE_WAVE 0x06

#define I8253_BASE_FREQUENCY 1193182

#define I8253_TIMER_IRQ 0

/* TODO: Don't expose this publicly */
extern TimerDevice i8253TimerDevice;

void i8253Initialize();
