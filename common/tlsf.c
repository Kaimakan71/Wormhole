/*
 * TLSF allocator services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <tlsf.h>

#if CONFIG_64BIT == 1
    #define FL_INDEX_MAX 32
    #define clz(x)       __builtin_clzll(x)
#else
    #define FL_INDEX_MAX 30
    #define clz(x)       __builtin_clz(x)
#endif

#define SL_INDEX_COUNT_LOG2 5
#define SL_INDEX_COUNT      BIT(SL_INDEX_COUNT_LOG2)

#define FL_INDEX_SHIFT (SL_INDEX_COUNT_LOG2 + TLSF_ALIGN_LOG2)
#define FL_INDEX_COUNT (FL_INDEX_MAX - FL_INDEX_SHIFT + 1)

#define SMALL_BLOCK_SIZE BIT(FL_INDEX_SHIFT)

#define BLOCK_HEADER_FREE      BIT(0)
#define BLOCK_HEADER_PREV_FREE BIT(1)

#define BLOCK_HEADER_OVERHEAD sizeof(uintn)
#define POOL_OVERHEAD         (2 * BLOCK_HEADER_OVERHEAD)

typedef struct BlockHeader {
    struct BlockHeader *prev;

    uintn size;

    struct BlockHeader *nextFree;
    struct BlockHeader *prevFree;
} BlockHeader;

#define BLOCK_SIZE_MIN (sizeof(BlockHeader) - sizeof(BlockHeader *))
#define BLOCK_SIZE_MAX BIT(FL_INDEX_MAX)

#define BLOCK_START_OFFSET (offsetof(BlockHeader, size) + sizeof(uintn))

typedef struct {
    BlockHeader nullBlock;

    uint32 flBitmap;
    uint32 slBitmap[FL_INDEX_COUNT];

    BlockHeader *blocks[FL_INDEX_COUNT][SL_INDEX_COUNT];
} Control;

static inline intn ffs(uintn word) {
    return __builtin_ffs(word) - 1;
}

static inline intn fls(uintn word) {
    return (word ? ((sizeof(word) * 8) - clz(word)) : 0) - 1;
}

static inline BlockHeader *offsetToBlock(const void *ptr, uintn off) {
    return (BlockHeader *) ((uintn) ptr + off);
}

static inline BlockHeader *ptrToBlock(const void *ptr) {
    return (BlockHeader *) ((uintn) ptr - BLOCK_START_OFFSET);
}

static inline void *blockToPtr(const BlockHeader *block) {
    return (void *) ((uintn) block + BLOCK_START_OFFSET);
}

static inline bool blockIsFree(const BlockHeader *block) {
    return block->size & BLOCK_HEADER_FREE;
}

static inline bool blockIsPrevFree(const BlockHeader *block) {
    return block->size & BLOCK_HEADER_PREV_FREE;
}

static inline uintn blockSize(const BlockHeader *block) {
    return block->size & ~(BLOCK_HEADER_FREE | BLOCK_HEADER_PREV_FREE);
}

static inline bool blockCanSplit(BlockHeader *block, uintn size) {
    return blockSize(block) >= (sizeof(BlockHeader) + size);
}

static inline BlockHeader *blockPrev(const BlockHeader *block) {
    return block->prev;
}

static inline BlockHeader *blockNext(const BlockHeader *block) {
    return offsetToBlock(blockToPtr(block), blockSize(block) - BLOCK_HEADER_OVERHEAD);
}

static inline void blockSetSize(BlockHeader *block, uintn size) {
    block->size = size | (block->size & (BLOCK_HEADER_FREE | BLOCK_HEADER_PREV_FREE));
}

static inline void blockSetFree(BlockHeader *block) {
    SET_BIT(block->size, BLOCK_HEADER_FREE);
}

static inline void blockSetUsed(BlockHeader *block) {
    CLEAR_BIT(block->size, BLOCK_HEADER_FREE);
}

static inline void blockSetPrevFree(BlockHeader *block) {
    SET_BIT(block->size, BLOCK_HEADER_PREV_FREE);
}

static inline void blockSetPrevUsed(BlockHeader *block) {
    CLEAR_BIT(block->size, BLOCK_HEADER_PREV_FREE);
}

static uintn adjustRequestSize(uintn size, uintn align) {
    if (size == 0) {
        return 0;
    }

    size = ALIGN_UP(size, align);
    if (size < BLOCK_SIZE_MAX) {
        size = max(size, BLOCK_SIZE_MIN);
    }

    return size;
}

static BlockHeader *blockLinkNext(BlockHeader *block) {
    BlockHeader *next;

    next = blockNext(block);
    next->prev = block;

    return next;
}

static void blockMarkFree(BlockHeader *block) {
    blockSetPrevFree(blockLinkNext(block));
    blockSetFree(block);
}

static void blockMarkUsed(BlockHeader *block) {
    blockSetPrevUsed(blockLinkNext(block));
    blockSetUsed(block);
}

static void mappingInsert(uintn size, intn *fli, intn *sli) {
    intn fl, sl;

    if (size < SMALL_BLOCK_SIZE) {
        fl = 0;
        sl = size / (SMALL_BLOCK_SIZE / SL_INDEX_COUNT);
    } else {
        fl = fls(size);
        sl = (intn) (size >> (fl - SL_INDEX_COUNT_LOG2)) ^ BIT(SL_INDEX_COUNT_LOG2);
        fl -= FL_INDEX_SHIFT - 1;
    }

    *fli = fl;
    *sli = sl;
}

static void mappingSearch(uintn size, intn *fli, intn *sli) {
    if (size >= SMALL_BLOCK_SIZE) {
        size += BIT(fls(size) - SL_INDEX_COUNT_LOG2) - 1;
    }

    mappingInsert(size, fli, sli);
}

static void insertFreeBlock(Control *ctl, BlockHeader *block, uintn fl, uintn sl) {
    BlockHeader *cur;

    cur = ctl->blocks[fl][sl];
    block->nextFree = cur;
    block->prevFree = &ctl->nullBlock;
    cur->prevFree = block;

    ctl->blocks[fl][sl] = block;
    ctl->flBitmap |= BIT(fl);
    ctl->slBitmap[fl] |= BIT(sl);
}

static void removeFreeBlock(Control *ctl, BlockHeader *block, uintn fl, uintn sl) {
    BlockHeader *prev, *next;

    prev = block->prevFree;
    next = block->nextFree;
    next->prevFree = prev;
    prev->nextFree = next;

    if (ctl->blocks[fl][sl] == block) {
        ctl->blocks[fl][sl] = next;
        if (next == &ctl->nullBlock) {
            CLEAR_BIT(ctl->slBitmap[fl], BIT(sl));
            if (ctl->slBitmap[fl] == 0) {
                CLEAR_BIT(ctl->flBitmap, BIT(fl));
            }
        }
    }
}

static void blockInsert(Control *ctl, BlockHeader *block) {
    intn fl, sl;

    mappingInsert(blockSize(block), &fl, &sl);
    insertFreeBlock(ctl, block, fl, sl);
}

static void blockRemove(Control *ctl, BlockHeader *block) {
    intn fl, sl;

    mappingInsert(blockSize(block), &fl, &sl);
    removeFreeBlock(ctl, block, fl, sl);
}

static BlockHeader *blockSplit(BlockHeader *block, uintn size) {
    BlockHeader *rem;
    uintn remSize;

    rem = offsetToBlock(blockToPtr(block), size - BLOCK_HEADER_OVERHEAD);
    remSize = blockSize(block) - size - BLOCK_HEADER_OVERHEAD;

    blockSetSize(rem, remSize);
    blockSetSize(block, size);
    blockMarkFree(rem);

    return rem;
}

static BlockHeader *blockAbsorb(BlockHeader *prev, BlockHeader *block) {
    prev->size += blockSize(block) + BLOCK_HEADER_OVERHEAD;
    blockLinkNext(prev);
    return prev;
}

static BlockHeader *blockMergePrev(Control *ctl, BlockHeader *block) {
    BlockHeader *prev;

    if (blockIsPrevFree(block)) {
        prev = blockPrev(block);
        blockRemove(ctl, prev);
        block = blockAbsorb(prev, block);
    }

    return block;
}

static BlockHeader *blockMergeNext(Control *ctl, BlockHeader *block) {
    BlockHeader *next;

    next = blockNext(block);
    if (blockIsFree(next)) {
        blockRemove(ctl, next);
        block = blockAbsorb(block, next);
    }

    return block;
}

static void blockTrimUsed(Control *ctl, BlockHeader *block, uintn size) {
    BlockHeader *rem;

    if (!blockCanSplit(block, size)) {
        return;
    }

    rem = blockSplit(block, size);
    blockSetPrevUsed(rem);
    rem = blockMergeNext(ctl, rem);
    blockInsert(ctl, rem);
}

static void blockTrimFree(Control *ctl, BlockHeader *block, uintn size) {
    BlockHeader *rem;

    if (!blockCanSplit(block, size)) {
        return;
    }

    rem = blockSplit(block, size);
    blockLinkNext(block);
    blockSetPrevFree(rem);
    blockInsert(ctl, rem);
}

static BlockHeader *searchSuitableBlock(Control *ctl, intn *fli, intn *sli) {
    intn fl, sl;
    uint32 slMap, flMap;

    fl = *fli;
    sl = *sli;

    slMap = ctl->slBitmap[fl] & (~0U << sl);
    if (slMap == 0) {
        flMap = ctl->flBitmap & (~0U << (fl + 1));
        if (flMap == 0) {
            return 0;
        }

        fl = ffs(flMap);
        *fli = fl;
        slMap = ctl->slBitmap[fl];
    }

    sl = ffs(slMap);
    *sli = sl;

    return ctl->blocks[fl][sl];
}

static BlockHeader *blockLocateFree(Control *ctl, uintn size) {
    intn fl, sl;
    BlockHeader *block;

    if (size == 0) {
        return NULL;
    }

    mappingSearch(size, &fl, &sl);
    if (fl >= FL_INDEX_COUNT) {
        return NULL;
    }

    block = searchSuitableBlock(ctl, &fl, &sl);
    if (block == NULL) {
        return NULL;
    }

    removeFreeBlock(ctl, block, fl, sl);
    return block;
}

static void *blockPrepareUsed(Control *ctl, BlockHeader *block, uintn size) {
    if (block == NULL) {
        return NULL;
    }

    blockTrimFree(ctl, block, size);
    blockMarkUsed(block);

    return blockToPtr(block);
}

bool tlsfAddPool(void *tlsf, void *memory, uintn size) {
    uintn poolSize;
    BlockHeader *block, *next;

    poolSize = ALIGN_DOWN(size - POOL_OVERHEAD, TLSF_ALIGN);
    if (poolSize < BLOCK_SIZE_MIN || poolSize > BLOCK_SIZE_MAX) {
        return false;
    }

    /* Create main free block */
    block = offsetToBlock(memory, -BLOCK_HEADER_OVERHEAD);
    blockSetSize(block, poolSize);
    blockSetFree(block);
    blockSetPrevUsed(block);
    blockInsert(tlsf, block);

    /* Split off sentinel block */
    next = blockLinkNext(block);
    blockSetSize(next, 0);
    blockSetUsed(next);
    blockSetPrevFree(next);

    return true;
}

void *tlsfInitialize(void *memory, uintn size) {
    Control *ctl;

    if (size < sizeof(*ctl)) {
        return NULL;
    }

    if ((uintn) memory % TLSF_ALIGN) {
        return NULL;
    }

    ctl = memory;
    ctl->nullBlock.nextFree = &ctl->nullBlock;
    ctl->nullBlock.prevFree = &ctl->nullBlock;
    ctl->flBitmap = 0;
    for (uintn i = 0; i < FL_INDEX_COUNT; i++) {
        ctl->slBitmap[i] = 0;
        for (uintn j = 0; j < SL_INDEX_COUNT; j++) {
            ctl->blocks[i][j] = &ctl->nullBlock;
        }
    }

    if (!tlsfAddPool(ctl, ctl + 1, size - sizeof(*ctl))) {
        return NULL;
    }

    return ctl;
}

void *tlsfAllocate(void *tlsf, uintn size) {
    BlockHeader *block;

    size = adjustRequestSize(size, TLSF_ALIGN);
    block = blockLocateFree(tlsf, size);

    return blockPrepareUsed(tlsf, block, size);
}

void tlsfFree(void *tlsf, void *ptr) {
    BlockHeader *block;

    if (ptr == NULL) {
        return;
    }

    block = ptrToBlock(ptr);
    blockMarkFree(block);
    block = blockMergePrev(tlsf, block);
    block = blockMergeNext(tlsf, block);
    blockInsert(tlsf, block);
}

void *tlsfResize(void *tlsf, void *ptr, uintn size) {
    BlockHeader *block, *next;
    uintn curSize, combined, adjust;
    void *newPtr;

    if (ptr == NULL) {
        return tlsfAllocate(tlsf, size);
    }

    if (size == 0) {
        tlsfFree(tlsf, ptr);
        return NULL;
    }

    block = ptrToBlock(ptr);
    next = blockNext(block);

    curSize = blockSize(block);
    combined = curSize + blockSize(next) + BLOCK_HEADER_OVERHEAD;
    adjust = adjustRequestSize(size, TLSF_ALIGN);

    if (adjust > curSize && (!blockIsFree(next) || adjust > combined)) {
        newPtr = tlsfAllocate(tlsf, size);
        if (newPtr == NULL) {
            return NULL;
        }

        /* TODO: Copy over old data */
        /* memcpy(newPtr, ptr, min(curSize, size)); */
        tlsfFree(tlsf, ptr);

        return newPtr;
    }

    if (adjust > curSize) {
        blockMergeNext(tlsf, block);
        blockMarkUsed(block);
    }

    blockTrimUsed(tlsf, block, adjust);

    return ptr;
}
