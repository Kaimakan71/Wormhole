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

static Process *schedIdleProcess;
static Thread *schedIdleThread;

static TimerDevice *schedTimerDevice;

static __noreturn void schedIdle() {
    /* TODO: Use cpuHalt() */
    for (;;);
}

static CpuThreadState *schedBlock(Thread *thread, CpuThreadState *threadState) {
    Thread *nextThread;

    /* Save current thread */
    thread->cpuState = threadState;

    /* Select next thread */
    if (!listIsEmpty(&schedRunnableThreads)) {
        nextThread = (Thread *) ((uintn) schedRunnableThreads.first - offsetof(Thread, schedListItem));
        listItemRemove(&nextThread->schedListItem);
    } else {
        /* Nothing better to switch to */
        nextThread = schedIdleThread;
    }

    /* Switch to next thread */
    traceDebug("TID %u blocked, switching to %u...\n", thread->id, nextThread->id);
    threadArchSetCurrent(nextThread);
    return nextThread->cpuState;
}

static CpuThreadState *schedTick(CpuThreadState *threadState) {
    Thread *currentThread, *nextThread;

    if (listIsEmpty(&schedRunnableThreads)) {
        /* Nothing better to switch to */
        return threadState;
    }

    /* Save current thread */
    currentThread = threadArchGetCurrent();
    currentThread->cpuState = threadState;
    if (currentThread != schedIdleThread) {
        listAppend(&schedRunnableThreads, &currentThread->schedListItem);
    }

    /* Select next thread */
    nextThread = (Thread *) ((uintn) schedRunnableThreads.first - offsetof(Thread, schedListItem));
    listItemRemove(&nextThread->schedListItem);

    /* Switch to next thread */
    traceDebug("TID %u preempted, switching to %u...\n", currentThread->id, nextThread->id);
    threadArchSetCurrent(nextThread);
    return nextThread->cpuState;
}

Status schedInitialize() {
    traceDebug("Initializing scheduler...\n");

    listReset(&schedRunnableThreads);

    schedIdleProcess = processCreate("idle", (void *) schedIdle);
    if (schedIdleProcess == NULL) {
        traceError("Failed to create idle process\n");
        return STATUS_UNKNOWN_ERROR;
    }

    schedIdleThread = (Thread *) schedIdleProcess->threads.first;
    threadArchSetCurrent(schedIdleThread);

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

CpuThreadState *schedThreadExit(Thread *thread, CpuThreadState *threadState) {
    threadState = schedBlock(thread, threadState);

    traceDebug("TID %u exited\n", thread->id);
    threadDestroy(thread);

    return threadState;
}
