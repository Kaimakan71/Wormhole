/*
 * PCI bus driver.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <bus/pci.h>
#include <cpu/pmio.h>
#include <log.h>
#include <mm.h>

#define TRACE_SOURCE "pci"

static List devices;

static inline void pciConfigSelect(uint8 bus, uint8 slot, uint8 function, uint8 offset) {
    pmioWrite32(PCI_CONFIG_ADDRESS, PCI_CONFIG_ADDRESS_ENABLE | (bus << 16) | (slot << 11) | (function << 8) | (offset & 0xfc));
}

static inline uint8 pciConfigRead8(uint8 bus, uint8 slot, uint8 function, uint8 offset) {
    pciConfigSelect(bus, slot, function, offset);
    return pmioRead8(PCI_CONFIG_DATA + (offset & 3));
}

static inline uint16 pciConfigRead16(uint8 bus, uint8 slot, uint8 function, uint8 offset) {
    pciConfigSelect(bus, slot, function, offset);
    return pmioRead16(PCI_CONFIG_DATA + (offset & 2));
}

static inline uint32 pciConfigRead32(uint8 bus, uint8 slot, uint8 function, uint8 offset) {
    pciConfigSelect(bus, slot, function, offset);
    return pmioRead32(PCI_CONFIG_DATA);
}

static inline uint16 pciGetVendorId(uint8 bus, uint8 slot, uint8 function) {
    return pciConfigRead16(bus, slot, function, PCI_HEADER_VENDOR_ID);
}

static inline uint16 pciGetDeviceId(uint8 bus, uint8 slot, uint8 function) {
    return pciConfigRead16(bus, slot, function, PCI_HEADER_DEVICE_ID);
}

static inline uint8 pciGetInterface(uint8 bus, uint8 slot, uint8 function) {
    return pciConfigRead8(bus, slot, function, PCI_HEADER_INTERFACE);
}

static inline uint8 pciGetSubclass(uint8 bus, uint8 slot, uint8 function) {
    return pciConfigRead8(bus, slot, function, PCI_HEADER_SUBCLASS);
}

static inline uint8 pciGetClass(uint8 bus, uint8 slot, uint8 function) {
    return pciConfigRead8(bus, slot, function, PCI_HEADER_CLASS);
}

static inline uint8 pciGetHeaderType(uint8 bus, uint8 slot, uint8 function) {
    return pciConfigRead8(bus, slot, function, PCI_HEADER_HEADER_TYPE);
}

static void scanFunction(uint8 bus, uint8 slot, uint8 function) {
    uint16 vendorId;
    PciDevice *dev;

    vendorId = pciGetVendorId(bus, slot, function);
    if (vendorId == PCI_VENDOR_ID_INVALID) {
        return;
    }

    dev = heapAllocate(sizeof(*dev));
    if (dev == NULL) {
        traceError("Failed to allocate memory for device descriptor\n");
        return;
    }

    dev->bus = bus;
    dev->slot = slot;
    dev->function = function;

    dev->vendorId = vendorId;
    dev->deviceId = pciGetDeviceId(bus, slot, function);
    dev->class = pciGetClass(bus, slot, function);
    dev->subclass = pciGetSubclass(bus, slot, function);
    dev->interface = pciGetInterface(bus, slot, function);

    listAppend(&devices, &dev->listItem);
}

static void scanSlot(uint8 bus, uint8 slot) {
    if (pciGetVendorId(bus, slot, 0) == PCI_VENDOR_ID_INVALID) {
        return;
    }

    scanFunction(bus, slot, 0);
    if (pciGetHeaderType(bus, slot, 0) & PCI_HEADER_TYPE_MULTIFUNCTION) {
        for (uint8 f = 1; f < 8; f++) {
            scanFunction(bus, slot, f);
        }
    }
}

static void scanBus(uint8 bus) {
    for (uint8 d = 0; d < 32; d++) {
        scanSlot(bus, d);
    }
}

void pciInitialize() {
    traceDebug("Initializing PCI driver...\n");

    listReset(&devices);
    scanBus(0);
    if (pciGetHeaderType(0, 0, 0) & PCI_HEADER_TYPE_MULTIFUNCTION) {
        for (uint8 f = 1; f < 8; f++) {
            if (pciGetVendorId(0, 0, f) == PCI_VENDOR_ID_INVALID) {
                break;
            }

            scanBus(f);
        }
    }
}

void pciEnumerateDevices(void (*callback)(const PciDevice *)) {
    PciDevice *dev;

    dev = (PciDevice *) devices.first;
    while (dev != (PciDevice *) &devices) {
        callback(dev);
        dev = (PciDevice *) dev->listItem.next;
    }
}

uint32 pciGetBar(const PciDevice *dev, uint8 index) {
    return pciConfigRead32(dev->bus, dev->slot, dev->function, PCI_HEADER_BAR0 + (index * 4));
}
