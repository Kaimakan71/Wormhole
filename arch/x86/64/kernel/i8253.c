/*
 * i8253 interval timer driver.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu/idt.h>
#include <cpu/i8253.h>
#include <cpu/pmio.h>
#include <log.h>

#define TRACE_SOURCE "i8253"

void i8253Initialize() {
    traceDebug("Initializing PIT...\n");
}
