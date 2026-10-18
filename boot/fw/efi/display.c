/*
 * EFI display services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "bootefi.h"

static EfiGraphicsOutputProtocol *gop = NULL;

Status displayInitialize(Framebuffer *fb) {
    Status status;
    EfiGraphicsOutputProtocolMode *mode;
    EfiGraphicsOutputModeInfo *modeInfo;

    consoleDebug(L"Initializing display...\r\n");

    status = efiStatusToStatus(efiLocateProtocol(&efiGraphicsOutputProtocol, (void **) &gop));
    if (!STATUS_IS_SUCCESS(status)) {
        consoleError(L"Failed to locate EFI graphics output protocol\r\n");
        return status;
    }

    mode = gop->mode;
    modeInfo = mode->info;
    fb->base = (uint32 *) mode->framebufferBase;
    fb->width = modeInfo->horizontalResolution;
    fb->height = modeInfo->verticalResolution;
    fb->pitch = modeInfo->pixelsPerScanLine;

    switch (modeInfo->pixelFormat) {
    case EFI_PIXEL_FORMAT_R8G8B8X8:
        fb->pixelFormat = PIXEL_FORMAT_R8G8B8X8;
        break;
    case EFI_PIXEL_FORMAT_B8G8R8X8:
        fb->pixelFormat = PIXEL_FORMAT_B8G8R8X8;
        break;
    default:
        consoleError(L"Unsupported framebuffer pixel format %32u\r\n", modeInfo->pixelFormat);
        return STATUS_FORMAT_ERROR;
    }

    status = mmMapPages((uintn) mode->framebufferBase, (uintn) mode->framebufferBase, SIZE_TO_PAGES(mode->framebufferSize));
    if (!STATUS_IS_SUCCESS(status)) {
        consoleError(L"Failed to map framebuffer at 0x%x\r\n", fb->base);
        return status;
    }

    return STATUS_SUCCESS;
}

void displayShutdown() {

}
