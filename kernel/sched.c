/*
 * Process scheduler.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu.h>
#include <cpu/i8253.h>
#include <log.h>
#include <sched.h>

#define TRACE_SOURCE "sched"

#define SCHED_TICK_FREQUENCY 1

static List schedRunnableThreads;

static TimerDevice *schedTimerDevice;

static __noreturn void schedIdle() {
    for (;;) {
        cpuHalt();
    }
}

static CpuThreadState *schedTick(CpuThreadState *threadState) {
    return threadState;
}

Status schedInitialize() {

    traceDebug("Initializing scheduler...\n");

    listReset(&schedRunnableThreads);

    /* TODO: Dynamically select timer device */
    schedTimerDevice = &i8253TimerDevice;

    return STATUS_SUCCESS;
}

void schedRegisterThread(Thread *thread) {
    listAppend(&schedRunnableThreads, &thread->schedListItem);
}

void schedRegisterProcess(Process *process) {
    ListItem *listItem;

    listEnumerateItems(&process->threads, listItem) {
        schedRegisterThread((Thread *) listItem);
    }
}

__noreturn void schedStart() {
    traceDebug("Starting scheduler...\n");

    schedTimerDevice->ops->setFrequency(schedTimerDevice, SCHED_TICK_FREQUENCY);
    schedTimerDevice->onTick = schedTick;

    schedIdle();
}
