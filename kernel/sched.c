/*
 * Process scheduler.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu.h>
#include <log.h>
#include <process.h>
#include <sched.h>

#define TRACE_SOURCE "sched"

static List schedRunnableThreads;

static void schedIdle() {
    /* TODO: Run in kernel mode */
    traceDebug("Idling...\n");
    for (;;) {
        cpuPause();
    }
}

static void schedRegisterThread(Thread *thread) {
    listAppend(&schedRunnableThreads, &thread->schedListItem);
}

static void schedRegisterProcess(Process *process) {
    ListItem *listItem;

    listEnumerateItems(&process->threads, listItem) {
        schedRegisterThread((Thread *) listItem);
    }
}

Status schedInitialize() {
    Process *idleProcess;

    traceDebug("Initializing scheduler...\n");

    listReset(&schedRunnableThreads);

    idleProcess = processCreate("idle", (void *) schedIdle);
    if (idleProcess == NULL) {
        traceError("Failed to create idle process\n");
        return STATUS_UNKNOWN_ERROR;
    }

    schedRegisterProcess(idleProcess);

    return STATUS_SUCCESS;
}

Thread *schedSchedule(Thread *current) {
    Thread *next;

    if (listIsEmpty(&schedRunnableThreads)) {
        return current;
    }

    /* Get next runnable thread */
    next = (Thread *) ((uintn) schedRunnableThreads.first - offsetof(Thread, schedListItem));
    listItemRemove(&next->schedListItem);

    /* Mark current thread as runnable */
    if (current != NULL) {
        schedRegisterThread(current);
    }

    return next;
}
