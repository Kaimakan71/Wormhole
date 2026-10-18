/*
 * EFI firmware services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu.h>
#include "bootefi.h"

Status fwShutdown() {
    Status status;
    EfiMemoryDescriptor *map;
    uintn mapSize, mapKey, descriptorSize;
    uint32 descriptorVersion;

    consoleDebug(L"Exiting firmware environment...\r\n");

    fsShutdown();
    efiCloseProtocol(efiImageHandle, &efiLoadedImageProtocol);

    status = efiStatusToStatus(efiGetMemoryMap(&map, &mapSize, &mapKey, &descriptorSize, &descriptorVersion));
    if (!STATUS_IS_SUCCESS(status)) {
        consoleError(L"Failed to get EFI memory map\r\n");
        return status;
    }

    status = efiStatusToStatus(efiBootServices->exitBootServices(efiImageHandle, mapKey));
    if (!STATUS_IS_SUCCESS(status)) {
        consoleError(L"Failed to exit EFI boot services\r\n");
        return status;
    }

    cpuIntDisable();

    return STATUS_SUCCESS;
}
