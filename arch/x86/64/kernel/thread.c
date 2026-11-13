/*
 * x86-64 thread management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <mm.h>
#include <thread.h>

Thread *threadCreate(Process *parent, const char8 *name) {
    Thread *thread;

    thread = heapAllocate(sizeof(*thread));
    if (thread == NULL) {
        return NULL;
    }

    thread->name = name;
    thread->parent = parent;
    listAppend(&parent->threads, &thread->listItem);

    return thread;
}
