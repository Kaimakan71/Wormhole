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
static uintn totalPages;

static inline void bitmapSet(uintn idx) {
    SET_BIT(bitmap[idx >> BITMAP_WORD_SHIFT], BIT(idx & (BITMAP_WORD_SIZE - 1)));
}

static inline void bitmapClear(uintn idx) {
    CLEAR_BIT(bitmap[idx >> BITMAP_WORD_SHIFT], BIT(idx & (BITMAP_WORD_SIZE - 1)));
}

static void markUsed(uintn start, uintn count) {
    uintn idx, word;

    /* Align to word size */
    idx = start;
    while (idx % BITMAP_WORD_SIZE && count) {
        bitmapClear(idx);
        idx++;
        count--;
    }

    /* Set words */
    word = idx >> BITMAP_WORD_SHIFT;
    while (count >= BITMAP_WORD_SIZE) {
        bitmap[word++] = 0;
        count -= BITMAP_WORD_SIZE;
    }
    idx = word << BITMAP_WORD_SHIFT;

    /* Finish up remainder */
    while (count) {
        bitmapClear(idx);
        idx++;
        count--;
    }
}

static void markFree(uintn start, uintn count) {
    uintn idx, word;

    /* Align to word size */
    idx = start;
    while (idx % BITMAP_WORD_SIZE && count) {
        bitmapSet(idx);
        idx++;
        count--;
    }

    /* Set words */
    word = idx >> BITMAP_WORD_SHIFT;
    while (count >= BITMAP_WORD_SIZE) {
        bitmap[word++] = ~0;
        count -= BITMAP_WORD_SIZE;
    }
    idx = word << BITMAP_WORD_SHIFT;

    /* Finish up remainder */
    while (count) {
        bitmapSet(idx);
        idx++;
        count--;
    }
}

Status pmmInitialize(MemoryRegion *memoryRegions, uintn memoryRegionCount) {
    MemoryRegion *region;
    uintn top, bitmapPages;

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
            region->physicalAddress += bitmapPages << PAGE_SHIFT;
            region->pageCount -= bitmapPages;
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

    /* Final iteration to mark free regions */
    region = &memoryRegions[0];
    for (uintn r = 0; r < memoryRegionCount; r++) {
        if (region->type == MEMORY_TYPE_FREE) {
            markFree(region->physicalAddress >> PAGE_SHIFT, region->pageCount);
        }

        region++;
    }

    return STATUS_SUCCESS;
}

Status pmmAllocate(uintn pageCount, uintn *physicalAddress) {
    uintn start;
    intn found;

    /* Locate free pages */
    found = -1;
    for (uintn p = 0; p < totalPages; p++) {
        if (bitmap[p >> BITMAP_WORD_SHIFT] & BIT(p & (BITMAP_WORD_SIZE - 1))) {
            if (found < 0) {
                start = p;
                found = 0;
            }

            found++;
            if ((uintn) found >= pageCount) {
                break;
            }
        } else {
            found = -1;
        }
    }
    if (found < 0 || (uintn) found != pageCount) {
        return STATUS_ALLOCATION_FAILED;
    }

    markUsed(start, pageCount);

    *physicalAddress = start << PAGE_SHIFT;
    return STATUS_SUCCESS;
}

void pmmFree(uintn physicalAddress, uintn pageCount) {
    markFree(physicalAddress >> PAGE_SHIFT, pageCount);
}
