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

#define USER_STACK_SIZE   0x4000
#define KERNEL_STACK_SIZE 0x1000

#define USER_STACK_PAGES   SIZE_TO_PAGES(USER_STACK_SIZE)
#define KERNEL_STACK_PAGES SIZE_TO_PAGES(KERNEL_STACK_SIZE)

static uintn nextThreadId = 0;

Thread *threadCreate(Process *parent, const char8 *name, void *entry) {
    Thread *thread;
    Status status;

    thread = heapAllocate(sizeof(*thread));
    if (thread == NULL) {
        traceError("Failed to allocate memory for thread descriptor\n");
        return NULL;
    }

    /* Initialize metadata */
    thread->id = nextThreadId++;
    thread->name = name;
    thread->parent = parent;

    /* Allocate user stack */
    thread->stackSize = USER_STACK_SIZE;
    status = vmmAllocatePages(USER_STACK_PAGES, &thread->stack);
    if (!STATUS_IS_SUCCESS(status)) {
        traceError("Failed to allocate %u page(s) for user stack\n", USER_STACK_PAGES);
        heapFree(thread);
        return NULL;
    }

    /* Allocate kernel stack */
    status = vmmAllocatePages(KERNEL_STACK_PAGES, &thread->kernelStack);
    if (!STATUS_IS_SUCCESS(status)) {
        traceError("Failed to allocate %u page(s) for kernel stack\n", KERNEL_STACK_PAGES);
        vmmFreePages(thread->stack, USER_STACK_PAGES);
        heapFree(thread);
        return NULL;
    }
    thread->kernelStackTop = (uintn) thread->kernelStack + KERNEL_STACK_SIZE;

    /* Initialize CPU-specific state */
    memClear(&thread->cpuState, sizeof(thread->cpuState));
    threadArchInitialize(thread, entry);

    /* Finalize registration */
    listAppend(&parent->threads, &thread->listItem);
    traceDebug("Created thread \"%s\" (TID %u)\n", thread->name, thread->id);

    return thread;
}

void threadDestroy(Thread *thread) {
    listItemRemove(&thread->listItem);

    vmmFreePages(thread->stack, USER_STACK_PAGES);
    vmmFreePages(thread->kernelStack, KERNEL_STACK_PAGES);

    /* TODO: Destroy parent process if necessary */

    heapFree(thread);
}
