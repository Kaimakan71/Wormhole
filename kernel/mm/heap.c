/*
 * SLUB heap allocator.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <log.h>
#include <mm.h>

#define TRACE_SOURCE "heap"

#define SLAB_PAGES 16
#define SLAB_SIZE  (SLAB_PAGES << PAGE_SHIFT)

typedef struct {
    union {
        void *nextFree;
        uint8 data[];
    };
} Object;

static Object *testSlab;

static void slabInitialize(Object *slab, uintn objectSize) {
    Object *obj, *next;

    obj = slab;
    for (uintn o = 0; o < (SLAB_SIZE / objectSize) - 1; o++) {
        next = (Object *) ((uintn) obj + objectSize);
        obj->nextFree = next;
        obj = next;
    }
    obj->nextFree = NULL;
}

static void *slabAllocate(Object *slab) {
    Object *obj;

    obj = slab->nextFree;
    if (obj == NULL) {
        return NULL;
    }

    slab->nextFree = obj->nextFree;

    return obj;
}

Status mmHeapInitialize() {
    Status status;
    uintn phys;

    traceDebug("Initializing heap...\n");

    status = pmmAllocate(SLAB_PAGES, &phys);
    if (!STATUS_IS_SUCCESS(status)) {
        traceError("Failed to allocate %u page(s) for slab\n", SLAB_PAGES);
        return status;
    }

    testSlab = (Object *) physToVirt(phys);
    slabInitialize(testSlab, 16);

    return STATUS_SUCCESS;
}

void *mmHeapAllocate(uintn size) {
    (void) size;

    /* TODO: Use slab of object size size */

    return slabAllocate(testSlab);
}

void mmHeapFree(void *ptr) {
    Object *obj;

    /* TODO: Use slab containing ptr */

    obj = ptr;
    obj->nextFree = testSlab->nextFree;
    testSlab->nextFree = obj;
}
