/*
 * ELF image loading services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <boot.h>
#include <elf.h>

#define PTR_OFFSET(ptr, off) ((void *) ((uintn) (ptr) + (off)))

static inline void memCopy(void *dest, void *src, uintn n) {
    uint8 *d, *s;

    d = dest;
    s = src;
    while (n--) {
        *d++ = *s++;
    }
}

static inline void memClear(void *dest, uintn n) {
    uint8 *d;

    d = dest;
    while (n--) {
        *d++ = 0;
    }
}

static bool validateHeader(const ElfImageHeader *hdr) {
    if (hdr->magic != ELF_MAGIC) {
        consoleError(L"Invalid ELF magic 0x%32x\r\n", hdr->magic);
        return false;
    }

    if (hdr->elfVersion != ELF_VERSION || hdr->elfVersion2 != ELF_VERSION) {
        consoleError(L"Invalid ELF version\r\n");
        return false;
    }

    if (hdr->imageType != ELF_IMAGE_TYPE_EXECUTABLE) {
        consoleError(L"Unsupported ELF image type %16u\r\n", hdr->imageType);
        return false;
    }

    if (hdr->format != ELF_NATIVE_FORMAT) {
        consoleError(L"Unsupported ELF format %8u\r\n", hdr->format);
        return false;
    }

    if (hdr->endianness != ELF_ENDIANNESS_LITTLE) {
        consoleError(L"Unsupported ELF endianness %8u\r\n", hdr->endianness);
        return false;
    }

    return true;
}

static Status loadPhdr(void *image, ElfProgramHeader *phdr) {
    Status status;
    uintn pageCount, fill;
    void *data;

    pageCount = SIZE_TO_PAGES(phdr->loadedSize);
    status = mmAllocatePages(pageCount, phdr->flags & ELF_PHDR_FLAG_EXECUTABLE, &data);
    if (!STATUS_IS_SUCCESS(status)) {
        consoleError(L"Failed to allocate %u page(s) for program data\r\n", pageCount);
        return status;
    }

    status = mmMapPages(phdr->virtualAddress, (uintn) data, pageCount);
    if (!STATUS_IS_SUCCESS(status)) {
        consoleError(L"Failed to map %u page(s) of program data\r\n", pageCount);
        return status;
    }

    /* Load initialized data */
    if (phdr->dataSize != 0) {
        memCopy(data, PTR_OFFSET(image, phdr->dataOffset), phdr->dataSize);
    }

    /* Clear uninitialized data */
    fill = phdr->loadedSize - phdr->dataSize;
    if (fill != 0) {
        memClear(PTR_OFFSET(data, phdr->dataSize), fill);
    }

    return STATUS_SUCCESS;
}

Status imgLoad(void *image, uintn imageSize, uintn *entry) {
    Status status;
    ElfImageHeader *hdr;
    ElfProgramHeader *phdr;

    consoleDebug(L"Loading executable image...\r\n");

    if (imageSize < sizeof(*hdr)) {
        consoleError(L"Missing ELF image header\r\n");
        return STATUS_FORMAT_ERROR;
    }

    hdr = image;
    if (!validateHeader(hdr)) {
        return STATUS_FORMAT_ERROR;
    }

    phdr = PTR_OFFSET(image, hdr->phdrsOffset);
    for (uint16 h = 0; h < hdr->phdrCount; h++) {
        if (phdr->type == ELF_PHDR_TYPE_LOAD && phdr->loadedSize > 0) {
            status = loadPhdr(image, phdr);
            if (!STATUS_IS_SUCCESS(status)) {
                /* TODO: Unload previously loaded phdrs */
                return status;
            }
        }

        phdr = PTR_OFFSET(phdr, hdr->phdrSize);
    }

    *entry = hdr->entry;
    return STATUS_SUCCESS;
}
