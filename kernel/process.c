/*
 * Process management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <mm.h>
#include <thread.h>

Process *processCreate(const char8 *name) {
    Process *proc;
    Thread *mainThread;

    proc = heapAllocate(sizeof(*proc));
    if (proc == NULL) {
        return NULL;
    }

    proc->name = name;
    listReset(&proc->threads);

    mainThread = threadCreate(proc, "main");
    if (mainThread == NULL) {
        heapFree(proc);
        return NULL;
    }

    return proc;
}
