/*
 * x86-64 pagemap definitions.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

typedef uint64 PagemapEntry;

#define PAGEMAP_ENTRY_COUNT 512
#define PAGEMAP_ENTRY_SHIFT 9
#define PAGEMAP_ENTRY_MASK  0x1ff

#define PAGEMAP_ENTRY_ADDRESS_MASK 0x000ffffffffff000

#define PAGEMAP_ENTRY_PRESENT      BIT(0)
#define PAGEMAP_ENTRY_WRITABLE     BIT(1)
#define PAGEMAP_ENTRY_USER         BIT(2)
#define PAGEMAP_ENTRY_PWT          BIT(3)
#define PAGEMAP_ENTRY_PCD          BIT(4)
#define PAGEMAP_ENTRY_ACCESSED     BIT(5)
#define PAGEMAP_ENTRY_DIRTY        BIT(6)
#define PAGEMAP_ENTRY_PAGE_SIZE    BIT(7)
#define PAGEMAP_ENTRY_GLOBAL       BIT(8)
#define PAGEMAP_ENTRY_HLAT_RESTART BIT(11)
#define PAGEMAP_ENTRY_XD           BIT(63)
