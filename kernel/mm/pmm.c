/*
 * Physical memory manager services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <log.h>
#include <mm.h>

#define TRACE_SOURCE "pmm"

#if CONFIG_64BIT == 1
    #define BITMAP_WORD_SIZE  64
    #define BITMAP_WORD_SHIFT 6
    #define ctz(w) __builtin_ctzll(w)
#else
    #define BITMAP_WORD_SIZE  32
    #define BITMAP_WORD_SHIFT 5
    #define ctz(w) __builtin_ctz(w)
#endif

static uintn bitmapWords;
static uintn *bitmap;

static inline void bitmapSet(uintn idx) {
    SET_BIT(bitmap[idx >> BITMAP_WORD_SHIFT], BIT(idx & (BITMAP_WORD_SIZE - 1)));
}

static inline void bitmapClear(uintn idx) {
    CLEAR_BIT(bitmap[idx >> BITMAP_WORD_SHIFT], BIT(idx & (BITMAP_WORD_SIZE - 1)));
}

static intn bitmapFfs() {
    uintn word;

    for (uintn w = 0; w < bitmapWords; w++) {
        word = bitmap[w];
        if (word != 0) {
            return (w * BITMAP_WORD_SIZE) + ctz(word);
        }
    }

    return -1;
}

Status pmmInitialize(MemoryRegion *memoryRegions, uintn memoryRegionCount) {
    MemoryRegion *region;
    uintn totalPages, top, bitmapPages;

    traceDebug("Initializing PMM...\n");

    /* Compute total physical pages */
    totalPages = 0;
    region = &memoryRegions[0];
    for (uintn r = 0; r < memoryRegionCount; r++) {
        top = (region->physicalAddress >> PAGE_SHIFT) + region->pageCount;
        if (top > totalPages) {
            totalPages = top;
        }

        region++;
    }

    /* Allocate main bitmap */
    bitmapWords = ALIGN_UP(totalPages, BITMAP_WORD_SIZE) / BITMAP_WORD_SIZE;
    bitmapPages = SIZE_TO_PAGES(sizeof(uintn) * bitmapWords);
    bitmap = NULL;
    region = &memoryRegions[0];
    for (uintn r = 0; r < memoryRegionCount; r++) {
        if (region->pageCount >= bitmapPages) {
            bitmap = physToVirt(region->physicalAddress);
            break;
        }

        region++;
    }
    if (bitmap == NULL) {
        traceError("Failed to allocate %u page(s) for bitmap\n", bitmapPages);
        return STATUS_ALLOCATION_FAILED;
    }

    /* All memory is used by default */
    for (uintn w = 0; w < bitmapWords; w++) {
        bitmap[w] = 0;
    }

    return STATUS_SUCCESS;
}
