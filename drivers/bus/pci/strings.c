/*
 * PCI strings.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <bus/pci.h>

static const char8 *getClassStringUnclassified(uint8 subclass) {
    switch (subclass) {
    case PCI_SUBCLASS_UNCLASSIFIED_NON_VGA:
        return "Non-VGA unclassified device";
    case PCI_SUBCLASS_UNCLASSIFIED_VGA_COMPATIBLE:
        return "VGA compatible unclassified device";
    case PCI_SUBCLASS_UNCLASSIFIED_IMAGE_COPROCESSOR:
        return "Image coprocessor";
    default:
        return "Unclassified device";
    }
}

static const char8 *getClassStringMassStorage(uint8 subclass) {
    switch (subclass) {
    case PCI_SUBCLASS_MASS_STORAGE_SCSI:
        return "SCSI storage controller";
    case PCI_SUBCLASS_MASS_STORAGE_IDE:
        return "IDE interface";
    case PCI_SUBCLASS_MASS_STORAGE_FDC:
        return "Floppy disk controller";
    case PCI_SUBCLASS_MASS_STORAGE_IPI:
        return "IPI bus controller";
    case PCI_SUBCLASS_MASS_STORAGE_RAID:
        return "RAID bus controller";
    case PCI_SUBCLASS_MASS_STORAGE_ATA:
        return "ATA controller";
    case PCI_SUBCLASS_MASS_STORAGE_SATA:
        return "SATA controller";
    case PCI_SUBCLASS_MASS_STORAGE_SAS:
        return "Serial Attached SCSI controller";
    case PCI_SUBCLASS_MASS_STORAGE_NVM:
        return "Non-Volatile memory controller";
    case PCI_SUBCLASS_MASS_STORAGE_UFS:
        return "Universal Flash Storage controller";
    default:
        return "Mass storage controller";
    }
}

static const char8 *getClassStringNetwork(uint8 subclass) {
    switch (subclass) {
    case PCI_SUBCLASS_NETWORK_ETHERNET:
        return "Ethernet controller";
    case PCI_SUBCLASS_NETWORK_TOKEN_RING:
        return "Token ring network controller";
    case PCI_SUBCLASS_NETWORK_FDDI:
        return "FDDI network controller";
    case PCI_SUBCLASS_NETWORK_ATM:
        return "ATM network controller";
    case PCI_SUBCLASS_NETWORK_ISDN:
        return "ISDN controller";
    case PCI_SUBCLASS_NETWORK_WORLDFIP:
        return "WorldFip controller";
    case PCI_SUBCLASS_NETWORK_PICMG:
        return "PICMG controller";
    case PCI_SUBCLASS_NETWORK_INFINIBAND:
        return "InfiniBand controller";
    case PCI_SUBCLASS_NETWORK_FABRIC:
        return "Fabric controller";
    default:
        return "Network controller";
    }
}

static const char8 *getClassStringDisplay(uint8 subclass) {
    switch (subclass) {
    case PCI_SUBCLASS_DISPLAY_VGA_COMPATIBLE:
        return "VGA compatible controller";
    case PCI_SUBCLASS_DISPLAY_XGA:
        return "XGA compatible controller";
    case PCI_SUBCLASS_DISPLAY_3D:
        return "3D controller";
    default:
        return "Display controller";
    }
}

static const char8 *getClassStringMultimedia(uint8 subclass) {
    switch (subclass) {
    case PCI_SUBCLASS_MULTIMEDIA_VIDEO:
        return "Multimedia video controller";
    case PCI_SUBCLASS_MULTIMEDIA_AUDIO:
        return "Multimedia audio controller";
    case PCI_SUBCLASS_MULTIMEDIA_COMPUTER_TELEPHONY:
        return "Computer telephony device";
    case PCI_SUBCLASS_MULTIMEDIA_AUDIO_DEVICE:
        return "Audio device";
    default:
        return "Multimedia controller";
    }
}

static const char8 *getClassStringMemory(uint8 subclass) {
    switch (subclass) {
    case PCI_SUBCLASS_MEMORY_RAM:
        return "RAM memory";
    case PCI_SUBCLASS_MEMORY_FLASH:
        return "FLASH memory";
    case PCI_SUBCLASS_MEMORY_CXL:
        return "CXL";
    default:
        return "Memory controller";
    }
}

static const char8 *getClassStringBridge(uint8 subclass) {
    switch (subclass) {
    case PCI_SUBCLASS_BRIDGE_HOST:
        return "Host bridge";
    case PCI_SUBCLASS_BRIDGE_ISA:
        return "ISA bridge";
    case PCI_SUBCLASS_BRIDGE_EISA:
        return "EISA bridge";
    case PCI_SUBCLASS_BRIDGE_MCA:
        return "MicroChannel bridge";
    case PCI_SUBCLASS_BRIDGE_PCI:
        return "PCI bridge";
    case PCI_SUBCLASS_BRIDGE_PCMCIA:
        return "PCMCIA bridge";
    case PCI_SUBCLASS_BRIDGE_NUBUS:
        return "NuBus bridge";
    case PCI_SUBCLASS_BRIDGE_CARDBUS:
        return "CardBus bridge";
    case PCI_SUBCLASS_BRIDGE_RACEWAY:
        return "RACEway bridge";
    case PCI_SUBCLASS_BRIDGE_PCI_TO_PCI_ST:
        return "Semi-transparent PCI-to-PCI bridge";
    case PCI_SUBCLASS_BRIDGE_INFINIBAND_HOST:
        return "InfiniBand to PCI host bridge";
    default:
        return "Bridge";
    }
}

static const char8 *getClassStringSerialBus(uint8 subclass) {
    switch (subclass) {
    case PCI_SUBCLASS_SERIAL_FIREWIRE:
        return "FireWire (IEEE 1394)";
    case PCI_SUBCLASS_SERIAL_ACCESS:
        return "ACCESS bus";
    case PCI_SUBCLASS_SERIAL_SSA:
        return "SSA";
    case PCI_SUBCLASS_SERIAL_USB:
        return "USB controller";
    case PCI_SUBCLASS_SERIAL_FIBRE_CHANNEL:
        return "Fibre Channel";
    case PCI_SUBCLASS_SERIAL_SMBUS:
        return "SMBus";
    case PCI_SUBCLASS_SERIAL_SERIAL_INFINIBAND:
        return "InfiniBand";
    case PCI_SUBCLASS_SERIAL_IPMI:
        return "IPMI";
    case PCI_SUBCLASS_SERIAL_SERCOS:
        return "SERCOS";
    case PCI_SUBCLASS_SERIAL_CANBUS:
        return "CANBUS";
    case PCI_SUBCLASS_SERIAL_MIPI_I3C:
        return "MIPI I3C";
    default:
        return "Serial bus controller";
    }
}

const char8 *pciGetClassString(uint8 class, uint8 subclass) {
    switch (class) {
    case PCI_CLASS_UNCLASSIFIED:
        return getClassStringUnclassified(subclass);
    case PCI_CLASS_MASS_STORAGE:
        return getClassStringMassStorage(subclass);
    case PCI_CLASS_NETWORK:
        return getClassStringNetwork(subclass);
    case PCI_CLASS_DISPLAY:
        return getClassStringDisplay(subclass);
    case PCI_CLASS_MULTIMEDIA:
        return getClassStringMultimedia(subclass);
    case PCI_CLASS_MEMORY:
        return getClassStringMemory(subclass);
    case PCI_CLASS_BRIDGE:
        return getClassStringBridge(subclass);
    case PCI_CLASS_COMMUNICATION:
        return "Communication controller";
    case PCI_CLASS_SYSTEM_PERIPHERAL:
        return "Generic system peripheral";
    case PCI_CLASS_INPUT_DEVICE:
        return "Input device controller";
    case PCI_CLASS_DOCKING_STATION:
        return "Docking station";
    case PCI_CLASS_PROCESSOR:
        return "Processor";
    case PCI_CLASS_SERIAL_BUS:
        return getClassStringSerialBus(subclass);
    case PCI_CLASS_WIRELESS:
        return "Wireless controller";
    case PCI_CLASS_INTELLIGENT:
        return "Intelligent controller";
    case PCI_CLASS_SATELLITE_COMMUNICATIONS:
        return "Satellite communications controller";
    case PCI_CLASS_ENCRYPTION:
        return "Encryption controller";
    case PCI_CLASS_SIGNAL_PROCESSING:
        return "Signal processing controller";
    case PCI_CLASS_PROCESSING_ACCELERATOR:
        return "Processing accelerator";
    case PCI_CLASS_NON_ESSENTIAL:
        return "Non-Essential Instrumentation";
    case PCI_CLASS_COPROCESSOR:
        return "Coprocessor";
    case PCI_CLASS_UNASSIGNED:
        return "Unassigned class";
    default:
        return "Unknown";
    }
}
