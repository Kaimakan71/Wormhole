/*
 * EFI status code translation services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include "bootefi.h"

EfiStatus statusToEfiStatus(Status status) {
    switch (status) {
    case STATUS_SUCCESS:
        return EFI_STATUS_SUCCESS;
    default:
        return EFI_STATUS_ABORTED;
    }
}

Status efiStatusToStatus(EfiStatus efiStatus) {
    switch (efiStatus) {
    case EFI_STATUS_SUCCESS:
        return STATUS_SUCCESS;
    default:
        return STATUS_UNKNOWN_ERROR;
    }
}
