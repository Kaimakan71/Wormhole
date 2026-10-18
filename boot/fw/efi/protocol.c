/*
 * EFI protocol services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "bootefi.h"

const EfiGuid efiLoadedImageProtocol = EFI_LOADED_IMAGE_PROTOCOL_GUID;
const EfiGuid efiSimpleFilesystemProtocol = EFI_SIMPLE_FILESYSTEM_PROTOCOL_GUID;
const EfiGuid efiGraphicsOutputProtocol = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

EfiStatus efiOpenProtocol(EfiHandle handle, const EfiGuid *protocol, void **interface) {
    if (efiBootServices->header.revision >= EFI_REVISION_1_10) {
        return efiBootServices->openProtocol(
            handle,
            (EfiGuid *) protocol,
            interface,
            efiImageHandle,
            NULL,
            EFI_OPEN_PROTOCOL_GET_PROTOCOL
        );
    }

    return efiBootServices->handleProtocol(
        handle,
        (EfiGuid *) protocol,
        interface
    );
}

EfiStatus efiCloseProtocol(EfiHandle handle, const EfiGuid *protocol) {
    if (efiBootServices->header.revision >= EFI_REVISION_1_10) {
        return efiBootServices->closeProtocol(
            handle,
            (EfiGuid *) protocol,
            efiImageHandle,
            NULL
        );
    }

    return EFI_STATUS_SUCCESS;
}

EfiStatus efiLocateHandle(const EfiGuid *protocol, EfiHandle **handles) {
    uintn bufSize;
    EfiHandle *buf;
    EfiStatus status;

    bufSize = 0;
    buf = NULL;
    for (uintn a = 0; a < 5; a++) {
        status = efiBootServices->locateHandle(EFI_LOCATE_SEARCH_TYPE_BY_PROTOCOL, (EfiGuid *) protocol, NULL, &bufSize, buf);
        if (status == EFI_STATUS_SUCCESS) {
            *handles = buf;
            return status;
        }

        if (buf != NULL) {
            mmHeapFree(buf);
        }

        if (status != EFI_STATUS_BUFFER_TOO_SMALL) {
            return status;
        }

        buf = mmHeapAllocate(bufSize);
        if (buf == NULL) {
            consoleError(L"Failed to allocate memory for handle buffer\r\n");
            return EFI_STATUS_OUT_OF_RESOURCES;
        }
    }

    if (buf != NULL) {
        mmHeapFree(buf);
    }

    return EFI_STATUS_TIMEOUT;
}

EfiStatus efiLocateProtocol(const EfiGuid *protocol, void **interface) {
    EfiStatus status;
    EfiHandle *handles, handle;

    if (efiBootServices->header.revision >= EFI_REVISION_1_10) {
        return efiBootServices->locateProtocol((EfiGuid *) protocol, NULL, interface);
    }

    status = efiLocateHandle(protocol, &handles);
    if (status != EFI_STATUS_SUCCESS) {
        return status;
    }
    handle = handles[0];
    mmHeapFree(handles);

    return efiBootServices->handleProtocol(
        handle,
        (EfiGuid *) protocol,
        interface
    );
}
