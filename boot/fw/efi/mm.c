/*
 * EFI memory management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <bootmm.h>
#include <cpu/mm.h>
#include "bootefi.h"

static uint8 efiGetMemoryType(const EfiMemoryDescriptor *desc) {
    if (desc->attributes & EFI_MEMORY_ATTRIBUTE_RUNTIME) {
        return MEMORY_TYPE_FIRMWARE;
    }

    switch (desc->type) {
    case EFI_MEMORY_TYPE_BOOT_SERVICES_CODE:
    case EFI_MEMORY_TYPE_BOOT_SERVICES_DATA:
    case EFI_MEMORY_TYPE_CONVENTIONAL:
        return MEMORY_TYPE_FREE;
    case EFI_MEMORY_TYPE_PERSISTENT:
        return MEMORY_TYPE_PERSISTENT;
    default:
        return MEMORY_TYPE_RESERVED;
    }
}

static inline bool canMerge(const EfiMemoryDescriptor *desc, const MemoryRegion *region, uint8 memoryType) {
    uint64 regionSize;

    if (region == NULL) {
        return false;
    }

    if (memoryType != region->type) {
        return false;
    }

    regionSize = PAGES_TO_SIZE(region->pageCount);

    if (desc->physicalStart != (region->physicalAddress + regionSize)) {
        return false;
    }

    if (region->virtualAddress != region->physicalAddress &&
        desc->virtualStart != (region->virtualAddress + regionSize)) {
        return false;
    }

    return true;
}

static inline bool mustMap(const EfiMemoryDescriptor *desc) {
    EfiMemoryType memoryType;
    uint64 start, end;

    if (desc->attributes & EFI_MEMORY_ATTRIBUTE_RUNTIME) {
        return true;
    }

    memoryType = desc->type;
    if (memoryType == efiLoadedImage->imageCodeType ||
        memoryType == efiLoadedImage->imageDataType) {
        return true;
    }

    start = desc->physicalStart;
    end = start + EFI_PAGES_TO_SIZE(desc->pageCount);
    if (start <= mmArchStackPointer &&
        end >= mmArchStackPointer) {
        return true;
    }

    return false;
}

EfiStatus efiGetMemoryMap(EfiMemoryDescriptor **map, uintn *mapSize, uintn *mapKey, uintn *descriptorSize, uint32 *descriptorVersion) {
    uintn bufSize;
    EfiMemoryDescriptor *buf;
    EfiStatus status;

    bufSize = 0;
    buf = NULL;

    status = efiBootServices->getMemoryMap(&bufSize, buf, mapKey, descriptorSize, descriptorVersion);
    if (status != EFI_STATUS_BUFFER_TOO_SMALL) {
        return status;
    }

    for (uintn a = 0; a < 5; a++) {
        buf = mmHeapAllocate(bufSize);
        if (buf == NULL) {
            consoleError(L"Failed to allocate memory for memory map\r\n");
            return EFI_STATUS_OUT_OF_RESOURCES;
        }

        status = efiBootServices->getMemoryMap(&bufSize, buf, mapKey, descriptorSize, descriptorVersion);
        if (status == EFI_STATUS_SUCCESS) {
            *map = buf;
            *mapSize = bufSize;
            return status;
        }

        mmHeapFree(buf);

        if (status != EFI_STATUS_BUFFER_TOO_SMALL) {
            return status;
        }
    }

    return EFI_STATUS_TIMEOUT;
}

void efiTranslateMemoryMap(EfiMemoryDescriptor *map, uintn descriptorSize, uintn descriptorCount, MemoryRegion *regions, uintn *regionCountOut) {
    MemoryRegion *region, *prevRegion;
    uintn regionCount;
    EfiMemoryDescriptor *desc;
    uint8 type;

    region = &regions[0];
    prevRegion = NULL;
    regionCount = 0;

    desc = &map[0];
    while (descriptorCount--) {
        type = efiGetMemoryType(desc);

        if (type == MEMORY_TYPE_RESERVED) {
            /* Leave reserved regions as gaps */
        } else if (canMerge(desc, prevRegion, type)) {
            /* Merge with previous region */
            prevRegion->pageCount += desc->pageCount;
        } else {
            /* Create new region */
            region->physicalAddress = desc->physicalStart;
            region->virtualAddress = desc->virtualStart != 0 ? desc->virtualStart : desc->physicalStart;
            region->pageCount = desc->pageCount;
            region->type = type;
            prevRegion = region++;
            regionCount++;
        }

        desc = (EfiMemoryDescriptor *) ((uintn) desc + descriptorSize);
    }

    *regionCountOut = regionCount;
}

Status mmFwInitialize() {
    Status status;
    EfiMemoryDescriptor *map, *desc;
    uintn mapSize, mapKey, descriptorSize, top, totalPages;
    uint32 descriptorVersion;

    status = efiGetStatus(efiGetMemoryMap(&map, &mapSize, &mapKey, &descriptorSize, &descriptorVersion));
    if (!STATUS_IS_SUCCESS(status)) {
        consoleError(L"Failed to get EFI memory map\r\n");
        return status;
    }

    /* Map the boot manager itself */
    consoleDebug(L"Mapping boot manager...\r\n");
    desc = map;
    totalPages = 0;
    for (uintn d = 0; d < (mapSize / descriptorSize); d++) {
        top = (desc->physicalStart >> PAGE_SHIFT) + desc->pageCount;
        if (top > totalPages) {
            totalPages = top;
        }

        if (mustMap(desc)) {
            /* TODO: Map runtime regions at correct virtual address */
            status = mmMapPages(desc->physicalStart, desc->physicalStart, desc->pageCount);
            if (!STATUS_IS_SUCCESS(status)) {
                consoleError(L"Failed to map %64u page(s) at 0x%64x\r\n", desc->pageCount, desc->physicalStart);
                mmHeapFree(map);
                return status;
            }
        }

        desc = (EfiMemoryDescriptor *) ((uintn) desc + descriptorSize);
    }
    mmHeapFree(map);

    /* Map all physical memory */
    consoleDebug(L"Creating HHDM...\r\n");
    status = mmMapPages(HHDM_OFFSET, 0, totalPages);
    if (!STATUS_IS_SUCCESS(status)) {
        consoleError(L"Failed to map %u page(s) for HHDM\r\n", totalPages);
        return status;
    }

    return STATUS_SUCCESS;
}

Status mmAllocatePages(uintn pageCount, bool code, void **ptr) {
    EfiStatus efiStatus;
    EfiPhysicalAddress efiAddress;

    efiAddress = 0;
    efiStatus = efiBootServices->allocatePages(
        EFI_ALLOCATE_TYPE_ANY_PAGES,
        code ? efiLoadedImage->imageCodeType : efiLoadedImage->imageDataType,
        pageCount,
        &efiAddress
    );

    *ptr = (void *) efiAddress;
    return efiGetStatus(efiStatus);
}

void mmFreePages(void *ptr, uintn pageCount) {
    EfiStatus status;

    status = efiBootServices->freePages(
        (EfiPhysicalAddress) ptr,
        pageCount
    );
    if (status != EFI_STATUS_SUCCESS) {
        consoleWarning(L"Failed to free %u page(s) at 0x%x\r\n", pageCount, ptr);
    }
}
