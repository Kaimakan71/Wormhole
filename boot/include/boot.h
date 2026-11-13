/*
 * Boot manager definitions.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <bootinfo.h>
#include <status.h>

#define PAGE_SIZE  0x1000
#define PAGE_MASK  0xfff
#define PAGE_SHIFT 12

#define SIZE_TO_PAGES(size)  (((size) >> PAGE_SHIFT) + (((size) & PAGE_MASK) ? 1 : 0))
#define PAGES_TO_SIZE(pages) ((pages) << PAGE_SHIFT)

/*
 * Console services.
 */

void consolePrint(const char16 *format, ...);
void consoleDebug(const char16 *format, ...);
void consoleInfo(const char16 *format, ...);
void consoleWarning(const char16 *format, ...);
void consoleError(const char16 *format, ...);

/*
 * Memory management services.
 */

Status mmInitialize(uintn heapSize);
Status mmAllocatePages(uintn pageCount, bool code, void **ptr);
void mmFreePages(void *ptr, uintn pageCount);
Status mmMapPages(uintn virtualAddress, uintn physicalAddress, uintn pageCount);
void mmActivateMappings();
void *mmHeapAllocate(uintn size);
void mmHeapFree(void *ptr);
void mmShutdown();

/*
 * Display services.
 */

Status displayInitialize(Framebuffer *fb);

/*
 * Filesystem services.
 */

Status fsInitialize();
Status fsLoad(const char16 *path, void **data, uintn *size);
void fsShutdown();

/*
 * Image loading services.
 */

Status imageLoad(void *image, uintn imageSize, uintn *entry);

/*
 * Firmware exit services.
 */

Status fwExit(BootMemoryRegion **memoryRegions, uintn *regionCount);

/*
 * Boot library services.
 */

typedef struct {
    uintn heapSize;
} LibraryParameters;

Status libraryInitialize(LibraryParameters *parameters, BootInfo *bootInfo);
void libraryShutdown();

/*
 * Boot manager main routine.
 */

Status bootMain();
