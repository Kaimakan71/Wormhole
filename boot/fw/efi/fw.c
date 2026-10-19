/*
 * EFI firmware services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu.h>
#include "bootefi.h"

Status fwShutdown(MemoryRegion **memoryRegions, uintn *regionCount) {
    Status status;
    EfiMemoryDescriptor *map;
    uintn mapSize, mapKey, descriptorSize, descriptorCount;
    uint32 descriptorVersion;
    MemoryRegion *regions;

    consoleDebug(L"Exiting firmware environment...\r\n");

    fsShutdown();
    efiCloseProtocol(efiImageHandle, &efiLoadedImageProtocol);

    status = efiStatusToStatus(efiGetMemoryMap(&map, &mapSize, &mapKey, &descriptorSize, &descriptorVersion));
    if (!STATUS_IS_SUCCESS(status)) {
        consoleError(L"Failed to get EFI memory map\r\n");
        return status;
    }

    descriptorCount = mapSize / descriptorSize;
    regions = mmHeapAllocate(sizeof(*regions) * descriptorCount);
    if (regions == NULL) {
        consoleError(L"Failed to allocate memory for memory region descriptors\r\n");
        mmHeapFree(map);
        return STATUS_ALLOCATION_FAILED;
    }

    efiTranslateMemoryMap(map, descriptorSize, descriptorCount, regions, regionCount);
    *memoryRegions = regions;

    status = efiStatusToStatus(efiBootServices->exitBootServices(efiImageHandle, mapKey));
    if (!STATUS_IS_SUCCESS(status)) {
        consoleError(L"Failed to exit EFI boot services\r\n");
        return status;
    }

    cpuIntDisable();

    return STATUS_SUCCESS;
}
