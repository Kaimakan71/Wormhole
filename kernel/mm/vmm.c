/*
 * Virtual memory management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <mm.h>

Status vmmAllocatePages(uintn pageCount, void **ptr) {
    Status status;
    uint64 phys;

    status = pmmAllocatePages(pageCount, &phys);
    if (!STATUS_IS_SUCCESS(status)) {
        return status;
    }

    *ptr = physToVirt(phys);
    return STATUS_SUCCESS;
}
