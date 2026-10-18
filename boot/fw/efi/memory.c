/*
 * EFI memory management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "bootefi.h"

EfiStatus efiGetMemoryMap(EfiMemoryDescriptor **map, uintn *mapSize, uintn *mapKey, uintn *descriptorSize, uint32 *descriptorVersion) {
    uintn bufSize;
    EfiMemoryDescriptor *buf;
    EfiStatus status;

    bufSize = 0;
    buf = NULL;
    for (uintn a = 0; a < 5; a++) {
        status = efiBootServices->getMemoryMap(&bufSize, buf, mapKey, descriptorSize, descriptorVersion);
        if (status == EFI_STATUS_SUCCESS) {
            *map = buf;
            *mapSize = bufSize;
            return status;
        }

        if (buf != NULL) {
            mmHeapFree(buf);
            buf = NULL;
        }

        if (status != EFI_STATUS_BUFFER_TOO_SMALL) {
            return status;
        }

        buf = mmHeapAllocate(bufSize);
        if (buf == NULL) {
            consoleError(L"Failed to allocate memory for memory map\r\n");
            return EFI_STATUS_OUT_OF_RESOURCES;
        }
    }

    if (buf != NULL) {
        mmHeapFree(buf);
    }

    return EFI_STATUS_TIMEOUT;
}

Status mmAllocatePages(uintn pageCount, bool executable, void **ptr) {
    EfiStatus efiStatus;
    EfiPhysicalAddress efiAddress;

    efiAddress = 0;
    efiStatus = efiBootServices->allocatePages(
        EFI_ALLOCATE_TYPE_ANY_PAGES,
        executable ? efiLoadedImage->imageCodeType : efiLoadedImage->imageDataType,
        pageCount,
        &efiAddress
    );

    *ptr = (void *) efiAddress;
    return efiStatusToStatus(efiStatus);
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
