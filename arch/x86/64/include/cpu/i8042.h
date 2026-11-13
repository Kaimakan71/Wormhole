/*
 * i8042 keyboard controller driver.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

#define I8042_DATA_PORT    0x60
#define I8042_COMMAND_PORT 0x64
#define I8042_STATUS_PORT  0x64

#define I8042_COMMAND_READ_CONFIG    0x20
#define I8042_COMMAND_WRITE_CONFIG   0x60
#define I8042_COMMAND_DISABLE_PORT_B 0xa7
#define I8042_COMMAND_ENABLE_PORT_B  0xa8
#define I8042_COMMAND_SELF_TEST      0xaa
#define I8042_COMMAND_DISABLE_PORT_A 0xad
#define I8042_COMMAND_ENABLE_PORT_A  0xae

#define I8042_STATUS_OUTPUT_FULL BIT(0)
#define I8042_STATUS_INPUT_FULL  BIT(1)

#define I8042_CONFIG_INT_ENABLE_A BIT(0)
#define I8042_CONFIG_INT_ENABLE_B BIT(0)

#define I8042_KEYBOARD_IRQ 1

void i8042Initialize();
