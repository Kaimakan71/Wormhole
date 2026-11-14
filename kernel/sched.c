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
    for (;;) {
        cpuPause();
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

    listAppend(&schedRunnableThreads, &((Thread *) idleProcess->threads.first)->schedListItem);

    return STATUS_SUCCESS;
}

Thread *schedSchedule() {
    Thread *thread;

    if (listIsEmpty(&schedRunnableThreads)) {
        return NULL;
    }

    thread = (Thread *) ((uintn) schedRunnableThreads.first - offsetof(Thread, schedListItem));
    listItemRemove(&thread->schedListItem);

    return thread;
}
