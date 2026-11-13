/*
 * EFI-specific boot manager services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <boot.h>
#include "efi.h"

extern const EfiGuid efiLoadedImageProtocol;
extern const EfiGuid efiGraphicsOutputProtocol;
extern const EfiGuid efiSimpleFilesystemProtocol;

extern EfiHandle efiImageHandle;
extern EfiSimpleTextOutputProtocol *efiConsoleOut;
extern EfiBootServices *efiBootServices;
extern EfiLoadedImageProtocol *efiLoadedImage;

/*
 * Status code services.
 */

Status efiGetStatus(EfiStatus efiStatus);
EfiStatus efiGetEfiStatus(Status status);

/*
 * Protocol services.
 */

EfiStatus efiOpenProtocol(EfiHandle handle, const EfiGuid *protocol, void **interface);
EfiStatus efiCloseProtocol(EfiHandle handle, const EfiGuid *protocol);
EfiStatus efiLocateHandle(const EfiGuid *protocol, EfiHandle **handles);
EfiStatus efiLocateProtocol(const EfiGuid *protocol, void **interface);

/*
 * Memory management services.
 */

EfiStatus efiGetMemoryMap(EfiMemoryDescriptor **map, uintn *mapSize, uintn *mapKey, uintn *descriptorSize, uint32 *descriptorVersion);
void efiTranslateMemoryMap(EfiMemoryDescriptor *map, uintn descriptorSize, uintn descriptorCount, MemoryRegion *regions, uintn *regionCountOut);
