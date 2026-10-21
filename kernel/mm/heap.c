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

#define MIN_ORDER 3
#define MAX_ORDER 8

#define MIN_OBJECT_SIZE BIT(MIN_ORDER)
#define MAX_OBJECT_SIZE BIT(MAX_ORDER)

#define ORDER_COUNT (MAX_ORDER - MIN_ORDER + 1)

#if CONFIG_64BIT == 1
    #define clz(w) __builtin_clzll(w)
#else
    #define clz(w) __builtin_clz(w)
#endif

typedef struct {
    union {
        void *nextFree;
        uint8 data[];
    };
} Object;

static Object *slabs[ORDER_COUNT];

static inline uintn log2(uintn size) {
    return (8 * sizeof(uintn)) - clz(size - 1);
}

static inline void *slabAllocate(Object *slab) {
    Object *obj;

    obj = slab->nextFree;
    if (obj == NULL) {
        return NULL;
    }

    slab->nextFree = obj->nextFree;

    return obj;
}

static inline void slabFree(Object *slab, Object *obj) {
    obj->nextFree = slab->nextFree;
    slab->nextFree = obj;
}

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

Status mmHeapInitialize() {
    Status status;
    uintn phys;
    Object *slab;

    traceDebug("Initializing heap...\n");

    for (uintn o = 0; o < ORDER_COUNT; o++) {
        status = pmmAllocate(SLAB_PAGES, &phys);
        if (!STATUS_IS_SUCCESS(status)) {
            traceError("Failed to allocate %u page(s) for slab\n", SLAB_PAGES);
            return status;
        }

        slab = (Object *) physToVirt(phys);
        slabInitialize(slab, BIT(MIN_ORDER + o));
        slabs[o] = slab;
    }

    return STATUS_SUCCESS;
}

void *mmHeapAllocate(uintn size) {
    if (size < MIN_OBJECT_SIZE) {
        size = MIN_OBJECT_SIZE;
    }

    if (size > MAX_OBJECT_SIZE) {
        return NULL;
    }

    return slabAllocate(slabs[log2(size) - MIN_ORDER]);
}

void mmHeapFree(void *ptr) {
    (void) ptr;

    /* TODO: Use slab containing ptr */
}
