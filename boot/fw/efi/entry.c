/*
 * EFI boot manager entry point.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "bootefi.h"

EfiHandle efiImageHandle;
EfiSimpleTextOutputProtocol *efiConsoleOut;
EfiBootServices *efiBootServices;
EfiLoadedImageProtocol *efiLoadedImage;

EfiStatus efiEntry(EfiHandle imageHandle, EfiSystemTable *systemTable) {
    EfiStatus status;

    efiImageHandle = imageHandle;
    efiConsoleOut = systemTable->consoleOut;
    efiBootServices = systemTable->bootServices;

    consoleInfo(L"Firmware vendor: \"%s\"\r\n", systemTable->firmwareVendor);
    consoleInfo(L"Firmware revision: 0x%32x\r\n", systemTable->firmwareRevision);

    status = efiOpenProtocol(efiImageHandle, &efiLoadedImageProtocol, (void **) &efiLoadedImage);
    if (status != EFI_STATUS_SUCCESS) {
        consoleError(L"Failed to open EFI loaded image protocol\r\n");
        return status;
    }

    consoleInfo(L"Image base: 0x%x\r\n", efiLoadedImage->imageBase);
    consoleInfo(L"Image size: 0x%64x\r\n", efiLoadedImage->imageSize);

    status = statusToEfiStatus(bootMain());

    efiCloseProtocol(efiImageHandle, &efiLoadedImageProtocol);

    return EFI_STATUS_SUCCESS;
}
