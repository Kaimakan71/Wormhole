/*
 * EFI filesystem services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "bootefi.h"

static const EfiGuid efiFileInfo = EFI_FILE_INFO_ID;

static EfiFileProtocol *root = NULL;

static EfiStatus close(EfiFileProtocol *file) {
    EfiStatus status;

    status = file->close(file);
    if (status != EFI_STATUS_SUCCESS) {
        consoleWarning(L"Failed to close file\r\n");
    }

    return status;
}

static EfiStatus getInfo(EfiFileProtocol *file, EfiFileInfo **info) {
    EfiStatus status;
    uintn bufSize;
    EfiFileInfo *buf;

    bufSize = 0;
    status = file->getInfo(file, (EfiGuid *) &efiFileInfo, &bufSize, NULL);
    if (status != EFI_STATUS_BUFFER_TOO_SMALL) {
        return status;
    }

    buf = mmHeapAllocate(bufSize);
    if (buf == NULL) {
        return status;
    }

    status = file->getInfo(file, (EfiGuid *) &efiFileInfo, &bufSize, buf);
    if (status != EFI_STATUS_SUCCESS) {
        mmHeapFree(buf);
        return status;
    }

    *info = buf;

    return EFI_STATUS_SUCCESS;
}

Status fsInitialize() {
    Status status;
    EfiSimpleFilesystemProtocol *fs;

    consoleDebug(L"Initializing filesystem...\r\n");

    status = efiStatusToStatus(efiOpenProtocol(
        efiLoadedImage->deviceHandle,
        (EfiGuid *) &efiSimpleFilesystemProtocol,
        (void **) &fs
    ));
    if (!STATUS_IS_SUCCESS(status)) {
        consoleError(L"Failed to open EFI simple filesystem protocol\r\n");
        return status;
    }

    status = efiStatusToStatus(fs->openVolume(fs, &root));
    efiCloseProtocol(efiImageHandle, (EfiGuid *) &efiSimpleFilesystemProtocol);
    if (!STATUS_IS_SUCCESS(status)) {
        consoleError(L"Failed to open root volume\r\n");
        return status;
    }

    return STATUS_SUCCESS;
}

Status fsLoad(const char16 *path, void **data, uintn *size) {
    Status status;
    EfiFileProtocol *file;
    EfiFileInfo *info;
    uintn bufSize;
    void *buf;

    consoleDebug(L"Loading file \"%s\"...\r\n", path);

    status = efiStatusToStatus(root->open(
        root,
        &file,
        (char16 *) path,
        EFI_FILE_MODE_READ,
        0
    ));
    if (!STATUS_IS_SUCCESS(status)) {
        consoleError(L"Failed to open file \"%s\"\r\n", path);
        return status;
    }

    status = efiStatusToStatus(getInfo(file, &info));
    if (!STATUS_IS_SUCCESS(status)) {
        consoleError(L"Failed to get info for file \"%s\"\r\n", path);
        close(file);
        return status;
    }
    bufSize = info->fileSize;
    mmHeapFree(info);

    buf = mmHeapAllocate(bufSize);
    if (buf == NULL) {
        consoleError(L"Failed to allocate buffer for file \"%s\"\r\n", path);
        close(file);
        return STATUS_ALLOCATION_FAILED;
    }

    status = efiStatusToStatus(file->read(file, &bufSize, buf));
    close(file);
    if (!STATUS_IS_SUCCESS(status)) {
        consoleError(L"Failed to read file \"%s\"\r\n", path);
        mmHeapFree(buf);
        return status;
    }

    *size = bufSize;
    *data = buf;

    return STATUS_SUCCESS;
}

void fsShutdown() {
    if (root != NULL) {
        close(root);
        root = NULL;
    }
}
