/*
 * EFI definitions.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

/*
 * Miscellaneous types.
 */

#define EFI_PAGE_SIZE  0x1000
#define EFI_PAGE_SHIFT 12

typedef void   *EfiHandle;
typedef uint64 EfiPhysicalAddress;
typedef uint64 EfiVirtualAddress;

typedef struct EfiSystemTable EfiSystemTable;

/*
 * EFI revision codes.
 */

#define EFI_MAKE_REVISION(major, minor) (((major) << 16) | (minor))

#define EFI_REVISION_1_02  EFI_MAKE_REVISION(1, 02)
#define EFI_REVISION_1_10  EFI_MAKE_REVISION(1, 10)
#define EFI_REVISION_2_00  EFI_MAKE_REVISION(2, 00)
#define EFI_REVISION_2_10  EFI_MAKE_REVISION(2, 10)
#define EFI_REVISION_2_20  EFI_MAKE_REVISION(2, 20)
#define EFI_REVISION_2_30  EFI_MAKE_REVISION(2, 30)
#define EFI_REVISION_2_31  EFI_MAKE_REVISION(2, 31)
#define EFI_REVISION_2_40  EFI_MAKE_REVISION(2, 40)
#define EFI_REVISION_2_50  EFI_MAKE_REVISION(2, 50)
#define EFI_REVISION_2_60  EFI_MAKE_REVISION(2, 60)
#define EFI_REVISION_2_70  EFI_MAKE_REVISION(2, 70)
#define EFI_REVISION_2_80  EFI_MAKE_REVISION(2, 80)
#define EFI_REVISION_2_90  EFI_MAKE_REVISION(2, 90)
#define EFI_REVISION_2_100 EFI_MAKE_REVISION(2, 100)
#define EFI_REVISION_2_110 EFI_MAKE_REVISION(2, 110)

/*
 * EFI status codes.
 */

typedef uintn EfiStatus;

#define EFI_STATUS_SUCCESS 0

#if CONFIG_64BIT == 1
    #define EFI_STATUS_ERROR_MASK 0x8000000000000000ULL
#else
    #define EFI_STATUS_ERROR_MASK 0x80000000
#endif
#define EFI_STATUS_ERROR(id) (EFI_STATUS_ERROR_MASK | (id))

/*
 * Standard errors.
 */

#define EFI_STATUS_LOAD_ERROR           EFI_STATUS_ERROR(1)
#define EFI_STATUS_INVALID_PARAMETER    EFI_STATUS_ERROR(2)
#define EFI_STATUS_UNSUPPORTED          EFI_STATUS_ERROR(3)
#define EFI_STATUS_BAD_BUFFER_SIZE      EFI_STATUS_ERROR(4)
#define EFI_STATUS_BUFFER_TOO_SMALL     EFI_STATUS_ERROR(5)
#define EFI_STATUS_NOT_READY            EFI_STATUS_ERROR(6)
#define EFI_STATUS_DEVICE_ERROR         EFI_STATUS_ERROR(7)
#define EFI_STATUS_WRITE_PROTECTED      EFI_STATUS_ERROR(8)
#define EFI_STATUS_OUT_OF_RESOURCES     EFI_STATUS_ERROR(9)
#define EFI_STATUS_VOLUME_CORRUPTED     EFI_STATUS_ERROR(10)
#define EFI_STATUS_VOLUME_FULL          EFI_STATUS_ERROR(11)
#define EFI_STATUS_NO_MEDIA             EFI_STATUS_ERROR(12)
#define EFI_STATUS_MEDIA_CHANGED        EFI_STATUS_ERROR(13)
#define EFI_STATUS_NOT_FOUND            EFI_STATUS_ERROR(14)
#define EFI_STATUS_ACCESS_DENIED        EFI_STATUS_ERROR(15)
#define EFI_STATUS_NO_RESPONSE          EFI_STATUS_ERROR(16)
#define EFI_STATUS_NO_MAPPING           EFI_STATUS_ERROR(17)
#define EFI_STATUS_TIMEOUT              EFI_STATUS_ERROR(18)
#define EFI_STATUS_NOT_STARTED          EFI_STATUS_ERROR(19)
#define EFI_STATUS_ALREADY_STARTED      EFI_STATUS_ERROR(20)
#define EFI_STATUS_ABORTED              EFI_STATUS_ERROR(21)
#define EFI_STATUS_ICMP_ERROR           EFI_STATUS_ERROR(22)
#define EFI_STATUS_TFTP_ERROR           EFI_STATUS_ERROR(23)
#define EFI_STATUS_PROTOCOL_ERROR       EFI_STATUS_ERROR(24)
#define EFI_STATUS_INCOMPATIBLE_VERSION EFI_STATUS_ERROR(25)
#define EFI_STATUS_SECURITY_VIOLATION   EFI_STATUS_ERROR(26)
#define EFI_STATUS_CRC_ERROR            EFI_STATUS_ERROR(27)
#define EFI_STATUS_END_OF_MEDIA         EFI_STATUS_ERROR(28)
#define EFI_STATUS_END_OF_FILE          EFI_STATUS_ERROR(31)
#define EFI_STATUS_INVALID_LANGUAGE     EFI_STATUS_ERROR(32)
#define EFI_STATUS_COMPROMISED_DATA     EFI_STATUS_ERROR(33)
#define EFI_STATUS_IP_ADDRESS_CONFLICT  EFI_STATUS_ERROR(34)
#define EFI_STATUS_HTTP_ERROR           EFI_STATUS_ERROR(35)

/*
 * TCP errors.
 */

#define EFI_STATUS_CONNECTION_FIN       EFI_STATUS_ERROR(104)
#define EFI_STATUS_CONNECTION_RESET     EFI_STATUS_ERROR(105)
#define EFI_STATUS_CONNECTION_REFUSED   EFI_STATUS_ERROR(106)

/*
 * ICMP errors.
 */

#define EFI_STATUS_NETWORK_UNREACHABLE  EFI_STATUS_ERROR(100)
#define EFI_STATUS_HOST_UNREACHABLE     EFI_STATUS_ERROR(101)
#define EFI_STATUS_PROTOCOL_UNREACHABLE EFI_STATUS_ERROR(102)
#define EFI_STATUS_PORT_UNREACHABLE     EFI_STATUS_ERROR(103)

/*
 * EFI memory types.
 */

typedef enum {
    EFI_MEMORY_TYPE_RESERVED,
    EFI_MEMORY_TYPE_LOADER_CODE,
    EFI_MEMORY_TYPE_LOADER_DATA,
    EFI_MEMORY_TYPE_BOOT_SERVICES_CODE,
    EFI_MEMORY_TYPE_BOOT_SERVICES_DATA,
    EFI_MEMORY_TYPE_RUNTIME_SERVICES_CODE,
    EFI_MEMORY_TYPE_RUNTIME_SERVICES_DATA,
    EFI_MEMORY_TYPE_CONVENTIONAL,
    EFI_MEMORY_TYPE_UNUSABLE,
    EFI_MEMORY_TYPE_ACPI_RECLAIMABLE,
    EFI_MEMORY_TYPE_ACPI_NVS,
    EFI_MEMORY_TYPE_MMIO,
    EFI_MEMORY_TYPE_MMIO_PORT_SPACE,
    EFI_MEMORY_TYPE_PAL_CODE,
    EFI_MEMORY_TYPE_PERSISTENT,
    EFI_MEMORY_TYPE_UNACCEPTED,
    EFI_MEMORY_TYPE_MAX,

    EFI_MEMORY_TYPE_OEM_RESERVED_MIN = 0x70000000,
    EFI_MEMORY_TYPE_OEM_RESERVED_MAX = 0x7fffffff,
    EFI_MEMORY_TYPE_OS_RESERVED_MIN  = 0x80000000,
    EFI_MEMORY_TYPE_OS_RESERVED_MAX  = 0xffffffff,
} EfiMemoryType;

/*
 * EFI memory attributes.
 */

#define EFI_MEMORY_ATTRIBUTE_UC            BIT(0)
#define EFI_MEMORY_ATTRIBUTE_WC            BIT(1)
#define EFI_MEMORY_ATTRIBUTE_WT            BIT(2)
#define EFI_MEMORY_ATTRIBUTE_WB            BIT(3)
#define EFI_MEMORY_ATTRIBUTE_UCE           BIT(4)
#define EFI_MEMORY_ATTRIBUTE_WP            BIT(12)
#define EFI_MEMORY_ATTRIBUTE_RP            BIT(13)
#define EFI_MEMORY_ATTRIBUTE_XP            BIT(14)
#define EFI_MEMORY_ATTRIBUTE_NV            BIT(15)
#define EFI_MEMORY_ATTRIBUTE_MORE_RELIABLE BIT(16)
#define EFI_MEMORY_ATTRIBUTE_RO            BIT(17)
#define EFI_MEMORY_ATTRIBUTE_SP            BIT(18)
#define EFI_MEMORY_ATTRIBUTE_CPU_CRYPTO    BIT(19)
#define EFI_MEMORY_ATTRIBUTE_ISA_VALID     BIT(62)
#define EFI_MEMORY_ATTRIBUTE_RUNTIME       BIT(63)

/*
 * EFI Globally Unique IDentifier (GUID).
 */

typedef struct {
    uint32 data1;
    uint16 data2;
    uint16 data3;
    uint8 data4[8];
} EfiGuid;

/*
 * EFI time.
 */

#define EFI_TIME_ADJUST_DAYLIGHT 0x01
#define EFI_TIME_IN_DAYLIGHT     0x02

#define EFI_UNSPECIFIED_TIMEZONE 0x07ff

typedef struct {
    uint16 year;
    uint8 month;
    uint8 day;
    uint8 hour;
    uint8 minute;
    uint8 second;
    uint8 pad1;
    uint32 nanosecond;
    int16 timezone;
    uint8 daylight;
    uint8 pad2;
} EfiTime;

/*
 * EFI simple text output protocol.
 */

typedef struct EfiSimpleTextOutputProtocol EfiSimpleTextOutputProtocol;

#define EFI_TEXT_MAKE_ATTRIBUTE(foreground, background) (((background) << 4) | (foreground))

#define EFI_COLOR_BLACK        0x00
#define EFI_COLOR_BLUE         0x01
#define EFI_COLOR_GREEN        0x02
#define EFI_COLOR_CYAN         0x03
#define EFI_COLOR_RED          0x04
#define EFI_COLOR_MAGENTA      0x05
#define EFI_COLOR_BROWN        0x06
#define EFI_COLOR_LIGHTGRAY    0x07
#define EFI_COLOR_BRIGHT       0x08
#define EFI_COLOR_DARKGRAY     0x08
#define EFI_COLOR_LIGHTBLUE    0x09
#define EFI_COLOR_LIGHTGREEN   0x0a
#define EFI_COLOR_LIGHTCYAN    0x0b
#define EFI_COLOR_LIGHTRED     0x0c
#define EFI_COLOR_LIGHTMAGENTA 0x0d
#define EFI_COLOR_YELLOW       0x0e
#define EFI_COLOR_WHITE        0x0f

typedef EfiStatus (EfiTextString)(EfiSimpleTextOutputProtocol *self, char16 *string);

typedef EfiStatus (EfiTextSetAttribute)(EfiSimpleTextOutputProtocol *self, uintn attribute);

struct EfiSimpleTextOutputProtocol {
    void *reset;

    EfiTextString *outputString;
    void *testString;

    void *queryMode;
    void *setMode;
    EfiTextSetAttribute *setAttribute;

    void *clearScreen;
    void *setCursorPosition;
    void *enableCursor;

    void *mode;
};

/*
 * EFI loaded image protocol.
 */

typedef struct EfiLoadedImageProtocol EfiLoadedImageProtocol;

#define EFI_LOADED_IMAGE_PROTOCOL_GUID { 0x5b1b31a1, 0x9562, 0x11d2, { 0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } }

#define EFI_LOADED_IMAGE_PROTOCOL_REVISION 0x1000

struct EfiLoadedImageProtocol {
    uint32 revision;
    EfiHandle parentHandle;
    EfiSystemTable *systemTable;

    EfiHandle deviceHandle;
    void *filePath;
    void *reserved;

    uint32 *loadOptionsSize;
    void *loadOptions;

    void *imageBase;
    uint64 imageSize;
    EfiMemoryType imageCodeType;
    EfiMemoryType imageDataType;
    void *unload;
};

/*
 * EFI file protocol.
 */

typedef struct EfiFileProtocol EfiFileProtocol;

#define EFI_FILE_PROTOCOL_REVISION        0x00010000
#define EFI_FILE_PROTOCOL_REVISION2       0x00020000
#define EFI_FILE_PROTOCOL_LATEST_REVISION EFI_FILE_PROTOCOL_REVISION2

#define EFI_FILE_MODE_READ   0x0000000000000001
#define EFI_FILE_MODE_WRITE  0x0000000000000002
#define EFI_FILE_MODE_CREATE 0x8000000000000000

#define EFI_FILE_ATTRIBUTE_READ_ONLY 0x0000000000000001
#define EFI_FILE_ATTRIBUTE_HIDDEN    0x0000000000000002
#define EFI_FILE_ATTRIBUTE_SYSTEM    0x0000000000000004
#define EFI_FILE_ATTRIBUTE_RESERVED  0x0000000000000008
#define EFI_FILE_ATTRIBUTE_DIRECTORY 0x0000000000000010
#define EFI_FILE_ATTRIBUTE_ARCHIVE   0x0000000000000020
#define EFI_VALID_FILE_ATTRIBUTES    0x0000000000000037

typedef EfiStatus (EfiFileOpen)(EfiFileProtocol *self, EfiFileProtocol **newHandle, char16 *filename, uint64 openMode, uint64 attributes);
typedef EfiStatus (EfiFileClose)(EfiFileProtocol *self);
typedef EfiStatus (EfiFileRead)(EfiFileProtocol *self, uintn *bufferSize, void *buffer);

#define EFI_FILE_INFO_ID { 0x09576e92, 0x6d3f, 0x11d2, { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } }

typedef struct {
    uint64 infoSize;
    uint64 fileSize;
    uint64 physicalSize;
    EfiTime creationTime;
    EfiTime lastAccessTime;
    EfiTime modificationTime;
    uint64 attributes;
    char16 name[];
} EfiFileInfo;

typedef EfiStatus (EfiFileGetInfo)(EfiFileProtocol *self, EfiGuid *infoType, uintn *bufferSize, void *buffer);

struct EfiFileProtocol {
    uint64 revision;

    EfiFileOpen *open;
    EfiFileClose *close;
    void *delete;
    EfiFileRead *read;
    void *write;
    void *getPosition;
    void *setPosition;
    EfiFileGetInfo *getInfo;
    void *setInfo;
    void *flush;

    /* Revision 2+ */
    void *openEx;
    void *readEx;
    void *writeEx;
    void *flushEx;
};

/*
 * EFI simple filesystem protocol.
 */

typedef struct EfiSimpleFilesystemProtocol EfiSimpleFilesystemProtocol;

#define EFI_SIMPLE_FILESYSTEM_PROTOCOL_GUID { 0x0964e5b22, 0x6459, 0x11d2, { 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b } }

#define EFI_SIMPLE_FILESYSTEM_PROTOCOL_REVISION 0x00010000

typedef EfiStatus (EfiSimpleFilesystemProtocolOpenVolume)(EfiSimpleFilesystemProtocol *this, EfiFileProtocol **root);

struct EfiSimpleFilesystemProtocol {
    uint64 revision;
    EfiSimpleFilesystemProtocolOpenVolume *openVolume;
};

/*
 * EFI graphics output protocol.
 */

#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID { 0x9042a9de, 0x23dc, 0x4a38, { 0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a } }

typedef struct EfiGraphicsOutputProtocol EfiGraphicsOutputProtocol;

typedef enum {
    EFI_PIXEL_FORMAT_R8G8B8X8,
    EFI_PIXEL_FORMAT_B8G8R8X8,
    EFI_PIXEL_FORMAT_BITMASK,
    EFI_PIXEL_FORMAT_BLT_ONLY,
    EFI_PIXEL_FORMAT_MAX,
} EfiPixelFormat;

typedef struct {
    uint32 redMask;
    uint32 greenMask;
    uint32 blueMask;
    uint32 reservedMask;
} EfiPixelBitmask;

typedef struct {
    uint32 version;
    uint32 horizontalResolution;
    uint32 verticalResolution;
    EfiPixelFormat pixelFormat;
    EfiPixelBitmask pixelBitmask;
    uint32 pixelsPerScanLine;
} EfiGraphicsOutputModeInfo;

typedef struct {
    uint32 maxMode;
    uint32 mode;
    EfiGraphicsOutputModeInfo *info;
    uintn infoSize;
    EfiPhysicalAddress framebufferBase;
    uintn framebufferSize;
} EfiGraphicsOutputProtocolMode;

struct EfiGraphicsOutputProtocol {
    void *queryMode;
    void *setMode;
    void *blt;
    EfiGraphicsOutputProtocolMode *mode;
};

/*
 * EFI configuration table.
 */

#define EFI_ACPI_10_TABLE_GUID { 0xeb9d2d30, 0x2d88, 0x11d3, { 0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d } }
#define EFI_ACPI_20_TABLE_GUID { 0x8868e871, 0xe4f1, 0x11d3, { 0xbc, 0x22, 0x00, 0x80, 0xc7, 0x3c, 0x88, 0x81 } }

typedef struct {
    EfiGuid vendorGuid;
    void *vendorTable;
} EfiConfigurationTable;

/*
 * EFI table header.
 */

typedef struct {
    uint64 signature;
    uint32 revision;
    uint32 headerSize;
    uint32 crc32;
    uint32 reserved;
} EfiTableHeader;

/*
 * EFI boot services table.
 */

#define EFI_BOOT_SERVICES_SIGNATURE 0x56524553544f4f42 /* "BOOTSERV" */

typedef enum {
    EFI_ALLOCATE_TYPE_ANY_PAGES,
    EFI_ALLOCATE_TYPE_MAX_ADDRESS,
    EFI_ALLOCATE_TYPE_ADDRESS,
    EFI_ALLOCATE_TYPE_MAX,
} EfiAllocateType;

typedef EfiStatus (EfiAllocatePages)(EfiAllocateType allocateType, EfiMemoryType memoryType, uintn pageCount, EfiPhysicalAddress *address);
typedef EfiStatus (EfiFreePages)(EfiPhysicalAddress address, uintn pageCount);

typedef struct {
    uint32 type;
    EfiPhysicalAddress physicalStart;
    EfiVirtualAddress virtualStart;
    uint64 pageCount;
    uint64 attributes;
} EfiMemoryDescriptor;

typedef EfiStatus (EfiGetMemoryMap)(uintn *mapSize, EfiMemoryDescriptor *map, uintn *key, uintn *descriptorSize, uint32 *descriptorVersion);

typedef EfiStatus (EfiAllocatePool)(EfiMemoryType type, uintn size, void **buffer);
typedef EfiStatus (EfiFreePool)(void *buffer);

typedef EfiStatus (EfiHandleProtocol)(EfiHandle handle, EfiGuid *protocol, void **interface);

typedef enum {
    EFI_LOCATE_SEARCH_TYPE_ALL_HANDLES,
    EFI_LOCATE_SEARCH_TYPE_BY_REGISTER_NOTIFY,
    EFI_LOCATE_SEARCH_TYPE_BY_PROTOCOL,
} EfiLocateSearchType;

typedef EfiStatus (EfiLocateHandle)(EfiLocateSearchType searchType, EfiGuid *protocol, void *searchKey, uintn *bufferSize, EfiHandle *buffer);

typedef EfiStatus (EfiExitBootServices)(EfiHandle imageHandle, uintn mapKey);

#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL  0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL        0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL       0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER           0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE           0x00000020

typedef EfiStatus (EfiOpenProtocol)(EfiHandle handle, EfiGuid *protocol, void **interface, EfiHandle agentHandle, EfiHandle controllerHandle, uint32 flags);
typedef EfiStatus (EfiCloseProtocol)(EfiHandle handle, EfiGuid *protocol, EfiHandle agentHandle, EfiHandle controllerHandle);

typedef EfiStatus (EfiLocateProtocol)(EfiGuid *protocol, void *registration, void **interface);

typedef struct {
    EfiTableHeader header;

    /*
     * Task Priority Level (TPL) services.
     */

    void *raiseTpl;
    void *restoreTpl;

    /*
     * Memory services.
     */

    EfiAllocatePages *allocatePages;
    EfiFreePages *freePages;
    EfiGetMemoryMap *getMemoryMap;
    EfiAllocatePool *allocatePool;
    EfiFreePool *freePool;

    /* 
     * Event & timer services.
     */

    void *createEvent;
    void *setTimer;
    void *waitForEvent;
    void *signalEvent;
    void *closeEvent;
    void *checkEvent;

    /*
     * Protocol handler services
     */

    void *installProtocolInterface;
    void *reinstallProtocolInterface;
    void *uninstallProtocolInterface;
    EfiHandleProtocol *handleProtocol;
    void *pcHandleProtocol;
    void *registerProtocolNotify;
    EfiLocateHandle *locateHandle;
    void *locateDevicePath;
    void *installConfigurationTable;

    /*
     * Image services.
     */

    void *loadImage;
    void *startImage;
    void *exit;
    void *unloadImage;
    EfiExitBootServices *exitBootServices;

    /*
     * System services.
     */

    void *getNextMonotonicCount;
    void *stall;
    void *setWatchdogTimer;

    /*
     * Driver services (EFI 1.1+).
     */

    void *connectController;
    void *disconnectController;

    /*
     * Protocol services (EFI 1.1+).
     */

    EfiOpenProtocol *openProtocol;
    EfiCloseProtocol *closeProtocol;
    void *openProtocolInformation;

    /*
     * Library services (EFI 1.1+).
     */

    void *protocolsPerHandle;
    void *locateHandleBuffer;
    EfiLocateProtocol *locateProtocol;
    void *installMultipleProtocolInterfaces;
    void *uninstallMultipleProtocolInterfaces;

    /*
     * Miscellaneous services (EFI 1.1+).
     */

    void *calculateCrc32;
    void *copyMemory;
    void *setMemory;
    void *createEventEx; /* UEFI 2.0+ */
} EfiBootServices;

/*
 * EFI system table.
 */

#define EFI_SYSTEM_TABLE_SIGNATURE 0x5453595320494249 /* "IBI SYST" */

struct EfiSystemTable {
    EfiTableHeader header;

    char16 *firmwareVendor;
    uint32 firmwareRevision;

    EfiHandle consoleInHandle;
    void *consoleIn;

    EfiHandle consoleOutHandle;
    EfiSimpleTextOutputProtocol *consoleOut;

    EfiHandle standardErrorHandle;
    EfiSimpleTextOutputProtocol *standardError;

    void *runtimeServices;
    EfiBootServices *bootServices;

    uintn configurationTableCount;
    EfiConfigurationTable *configurationTables;
};
