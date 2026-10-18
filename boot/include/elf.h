/*
 * ELF format definitions.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

#define ELF_MAGIC 0x464c457f

#define ELF_FORMAT_32BIT 1
#define ELF_FORMAT_64BIT 2

#define ELF_ENDIANNESS_LITTLE 1
#define ELF_ENDIANNESS_BIG    2

#define ELF_VERSION 1

#define ELF_IMAGE_TYPE_NONE          0x0000
#define ELF_IMAGE_TYPE_RELOCATABLE   0x0001
#define ELF_IMAGE_TYPE_EXECUTABLE    0x0002
#define ELF_IMAGE_TYPE_SHARED_OBJECT 0x0003
#define ELF_IMAGE_TYPE_CORE_DUMP     0x0004

typedef struct {
    uint32 magic;
    uint8 format;
    uint8 endianness;
    uint8 elfVersion;
    uint8 abi;
    uint8 abiVersion;
    uint8 pad[7];
    uint16 imageType;
    uint16 machine;
    uint32 elfVersion2;
    uintn entry;
    uintn phdrsOffset;
    uintn shdrsOffset;
    uint32 flags;
    uint16 headerSize;
    uint16 phdrSize;
    uint16 phdrCount;
    uint16 shdrSize;
    uint16 shdrCount;
    uint16 sectionNamesIndex;
} ElfImageHeader;

#define ELF_PHDR_TYPE_NULL    0x00000000
#define ELF_PHDR_TYPE_LOAD    0x00000001
#define ELF_PHDR_TYPE_DYNAMIC 0x00000002
#define ELF_PHDR_TYPE_INTERP  0x00000003
#define ELF_PHDR_TYPE_NOTE    0x00000004
#define ELF_PHDR_TYPE_SHLIB   0x00000005
#define ELF_PHDR_TYPE_PHDR    0x00000006
#define ELF_PHDR_TYPE_TLS     0x00000007

#define ELF_PHDR_FLAG_EXECUTABLE BIT(0)
#define ELF_PHDR_FLAG_WRITABLE   BIT(1)
#define ELF_PHDR_FLAG_READABLE   BIT(2)

typedef struct {
    uint32 type;
    uint32 dataOffset;
    uint32 virtualAddress;
    uint32 physicalAddress;
    uint32 dataSize;
    uint32 loadedSize;
    uint32 flags;
    uint32 align;
} Elf32ProgramHeader;

typedef struct {
    uint32 type;
    uint32 flags;
    uint64 dataOffset;
    uint64 virtualAddress;
    uint64 physicalAddress;
    uint64 dataSize;
    uint64 loadedSize;
    uint64 align;
} Elf64ProgramHeader;

#if CONFIG_64BIT == 1
    #define ELF_NATIVE_FORMAT ELF_FORMAT_64BIT
    typedef Elf64ProgramHeader ElfProgramHeader;
#else
    #define ELF_NATIVE_FORMAT ELF_FORMAT_32BIT
    typedef Elf32ProgramHeader ElfProgramHeader;
#endif
