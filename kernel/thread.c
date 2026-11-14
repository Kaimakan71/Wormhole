/*
 * Thread management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <log.h>
#include <memory.h>
#include <mm.h>
#include <thread.h>

#define TRACE_SOURCE "thread"

#define STACK_SIZE 0x1000

static uintn threadCount = 0;

Thread *threadCreate(Process *parent, const char8 *name) {
    Thread *thread;
    uintn pageCount;
    Status status;

    thread = heapAllocate(sizeof(*thread));
    if (thread == NULL) {
        return NULL;
    }

    /* Initialize metadata */
    thread->id = threadCount;
    thread->name = name;
    thread->parent = parent;

    /* Allocate stack */
    thread->stackSize = STACK_SIZE;
    pageCount = SIZE_TO_PAGES(thread->stackSize);
    status = vmmAllocatePages(pageCount, &thread->stack);
    if (!STATUS_IS_SUCCESS(status)) {
        traceError("Failed to allocate %u page(s) for thread stack\n", pageCount);
        heapFree(thread);
        return NULL;
    }

    /* Initialize CPU-specific state */
    memClear(&thread->cpuState, sizeof(thread->cpuState));
    threadArchInitialize(thread);

    /* Finalize registration */
    listAppend(&parent->threads, &thread->listItem);
    threadCount++;
    traceDebug("Created thread \"%s\" (TID %u)\n", thread->name, thread->id);

    return thread;
}
