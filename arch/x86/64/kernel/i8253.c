/*
 * i8253 interval timer driver.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu/idt.h>
#include <cpu/i8253.h>
#include <cpu/i8259.h>
#include <cpu/pmio.h>
#include <log.h>

#define TRACE_SOURCE "i8253"

extern void isr32();

static void i8253SetFrequency(TimerDevice *device, uint64 frequency) {
    uint16 divisor;

    (void) device;

    /* Set divisor */
    divisor = max(I8253_BASE_FREQUENCY / frequency, 1);
    outb(I8253_COMMAND_PORT, I8253_CHANNEL(0) | I8253_ACCESS_LOHI | I8253_MODE_SQUARE_WAVE);
    outb(I8253_CH0_DATA_PORT, divisor & 0xff);
    outb(I8253_CH0_DATA_PORT, (divisor >> 8) & 0xff);
}

static CpuThreadState *i8253DefaultCallback(CpuThreadState *cpuState) {
    return cpuState;
}

CpuThreadState *i8253TimerIsr(CpuThreadState *cpuState) {
    cpuState = i8253TimerDevice.onTick(cpuState);

    i8259Eoi(I8253_TIMER_IRQ);

    return cpuState;
}

void i8253Initialize() {
    traceDebug("Initializing PIT...\n");

    /* Disable channel 0 until needed */
    outb(I8253_COMMAND_PORT, I8253_CHANNEL(0) | I8253_ACCESS_LOHI | I8253_MODE_SQUARE_WAVE);
    outb(I8253_CH0_DATA_PORT, 0);
    outb(I8253_CH0_DATA_PORT, 0);
    outb(I8253_COMMAND_PORT, I8253_CHANNEL(0) | I8253_ACCESS_LOHI | I8253_MODE_SQUARE_WAVE);

    /* Register ISR */
    idtSetVector(I8253_TIMER_IRQ + IDT_IRQ_OFFSET, (uint32) isr32, 0, IDT_ENTRY_TYPE_32BIT | IDT_ENTRY_TYPE_INTERRUPT, 0);
    i8259Unmask(I8253_TIMER_IRQ);

    /* Register timer device */
    deviceRegister(&i8253TimerDevice.device);
}

static TimerDeviceOps i8253TimerOps = {
    .setFrequency = i8253SetFrequency,
};

TimerDevice i8253TimerDevice = {
    .device = {
        .name  = "i8253 Programmable Interval Timer",
        .class = &deviceClassTimer,
    },
    .ops = &i8253TimerOps,
    .onTick = i8253DefaultCallback,
};
