/*
 * EFI status code services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "bootefi.h"

Status efiGetStatus(EfiStatus efiStatus) {
    switch (efiStatus) {
    case EFI_STATUS_SUCCESS:
        return STATUS_SUCCESS;
    case EFI_STATUS_INVALID_PARAMETER:
        return STATUS_INVALID_ARGUMENT;
    case EFI_STATUS_UNSUPPORTED:
        return STATUS_UNSUPPORTED;
    case EFI_STATUS_OUT_OF_RESOURCES:
        return STATUS_ALLOCATION_FAILED;
    case EFI_STATUS_NOT_FOUND:
        return STATUS_NOT_FOUND;
    default:
        return STATUS_UNKNOWN_ERROR;
    }
}

EfiStatus efiGetEfiStatus(Status status) {
    switch (status) {
    case STATUS_SUCCESS:
        return EFI_STATUS_SUCCESS;
    case STATUS_INVALID_ARGUMENT:
        return EFI_STATUS_INVALID_PARAMETER;
    case STATUS_UNSUPPORTED:
        return EFI_STATUS_UNSUPPORTED;
    case STATUS_ALLOCATION_FAILED:
        return EFI_STATUS_OUT_OF_RESOURCES;
    case STATUS_NOT_FOUND:
        return EFI_STATUS_NOT_FOUND;
    default:
        return EFI_STATUS_ABORTED;
    }
}
