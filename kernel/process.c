/*
 * Process management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <log.h>
#include <mm.h>
#include <thread.h>

#define TRACE_SOURCE "process"

static uintn processCount = 0;

Process *processCreate(const char8 *name, uint8 threadFlags, void *entry) {
    Process *proc;
    Thread *mainThread;

    proc = heapAllocate(sizeof(*proc));
    if (proc == NULL) {
        traceError("Failed to allocate memory for process descriptor\n");
        return NULL;
    }

    /* Initialize metadata */
    proc->id = processCount;
    proc->name = name;
    listReset(&proc->threads);

    /* Create main thread */
    mainThread = threadCreate(proc, "main", threadFlags, entry);
    if (mainThread == NULL) {
        heapFree(proc);
        return NULL;
    }

    /* Finalize registration */
    processCount++;
    traceDebug("Created process \"%s\" (PID %u)\n", proc->name, proc->id);

    return proc;
}
