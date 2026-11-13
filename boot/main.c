/*
 * Boot manager main routine.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <boot.h>
#include <cpu.h>

#define HEAP_SIZE 0x10000

#define KERNEL_PATH L"\\kernel.elf"

static BootInfo bootInfo;

Status bootMain() {
    Status status;
    LibraryParameters libraryParams;
    void *img;
    uintn imgSize, imgEntry;
    __sysv_abi void (*osMain)(BootInfo *);

    consolePrint(L"Starting boot manager...\r\n");

    libraryParams.heapSize = HEAP_SIZE;
    status = libraryInitialize(&libraryParams, &bootInfo);
    if (!STATUS_IS_SUCCESS(status)) {
        goto exit;
    }

    status = fsLoad(KERNEL_PATH, &img, &imgSize);
    if (!STATUS_IS_SUCCESS(status)) {
        goto exit;
    }

    status = imageLoad(img, imgSize, &imgEntry);
    mmFreePages(img, SIZE_TO_PAGES(imgSize));
    if (!STATUS_IS_SUCCESS(status)) {
        goto exit;
    }

    /*
     * This is the point of no return.
     * Once we disable firmware services, we must either
     * transition to the OS or hang.
     */

    status = fwExit(&bootInfo.memoryRegions, &bootInfo.memoryRegionCount);
    if (!STATUS_IS_SUCCESS(status)) {
        goto exit;
    }

    mmActivateMappings();
    osMain = (void *) imgEntry;
    osMain(&bootInfo);
    cpuHang();

exit:
    libraryShutdown();
    return status;
}
