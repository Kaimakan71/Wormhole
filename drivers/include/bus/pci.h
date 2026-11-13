/*
 * PCI bus driver.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <device.h>

#define PCI_CONFIG_ADDRESS 0x0cf8
#define PCI_CONFIG_DATA    0x0cfc

#define PCI_CONFIG_ADDRESS_ENABLE BIT(31)

#define PCI_HEADER_VENDOR_ID       0x00
#define PCI_HEADER_DEVICE_ID       0x02
#define PCI_HEADER_COMMAND         0x04
#define PCI_HEADER_STATUS          0x06
#define PCI_HEADER_REVISION_ID     0x08
#define PCI_HEADER_INTERFACE       0x09
#define PCI_HEADER_SUBCLASS        0x0a
#define PCI_HEADER_CLASS           0x0b
#define PCI_HEADER_CACHE_LINE_SIZE 0x0c
#define PCI_HEADER_LATENCY_TIMER   0x0d
#define PCI_HEADER_HEADER_TYPE     0x0e
#define PCI_HEADER_BIST            0x0f

#define PCI_HEADER_BAR0 0x10
#define PCI_HEADER_BAR1 0x14
#define PCI_HEADER_BAR2 0x18
#define PCI_HEADER_BAR3 0x1c
#define PCI_HEADER_BAR4 0x20
#define PCI_HEADER_BAR5 0x24

#define PCI_VENDOR_ID_INVALID 0xffff

#define PCI_CLASS_UNCLASSIFIED             0x00
#define PCI_CLASS_MASS_STORAGE             0x01
#define PCI_CLASS_NETWORK                  0x02
#define PCI_CLASS_DISPLAY                  0x03
#define PCI_CLASS_MULTIMEDIA               0x04
#define PCI_CLASS_MEMORY                   0x05
#define PCI_CLASS_BRIDGE                   0x06
#define PCI_CLASS_COMMUNICATION            0x07
#define PCI_CLASS_SYSTEM_PERIPHERAL        0x08
#define PCI_CLASS_INPUT_DEVICE             0x09
#define PCI_CLASS_DOCKING_STATION          0x0a
#define PCI_CLASS_PROCESSOR                0x0b
#define PCI_CLASS_SERIAL_BUS               0x0c
#define PCI_CLASS_WIRELESS                 0x0d
#define PCI_CLASS_INTELLIGENT              0x0e
#define PCI_CLASS_SATELLITE_COMMUNICATIONS 0x0f
#define PCI_CLASS_ENCRYPTION               0x10
#define PCI_CLASS_SIGNAL_PROCESSING        0x11
#define PCI_CLASS_PROCESSING_ACCELERATOR   0x12
#define PCI_CLASS_NON_ESSENTIAL            0x13
#define PCI_CLASS_COPROCESSOR              0x40
#define PCI_CLASS_UNASSIGNED               0xff

/* Unclassified subclasses */
#define PCI_SUBCLASS_UNCLASSIFIED_NON_VGA           0x00
#define PCI_SUBCLASS_UNCLASSIFIED_VGA_COMPATIBLE    0x01
#define PCI_SUBCLASS_UNCLASSIFIED_IMAGE_COPROCESSOR 0x02

/* Mass storage controller subclasses */
#define PCI_SUBCLASS_MASS_STORAGE_SCSI 0x00
#define PCI_SUBCLASS_MASS_STORAGE_IDE  0x01
#define PCI_SUBCLASS_MASS_STORAGE_FDC  0x02
#define PCI_SUBCLASS_MASS_STORAGE_IPI  0x03
#define PCI_SUBCLASS_MASS_STORAGE_RAID 0x04
#define PCI_SUBCLASS_MASS_STORAGE_ATA  0x05
#define PCI_SUBCLASS_MASS_STORAGE_SATA 0x06
#define PCI_SUBCLASS_MASS_STORAGE_SAS  0x07
#define PCI_SUBCLASS_MASS_STORAGE_NVM  0x08
#define PCI_SUBCLASS_MASS_STORAGE_UFS  0x09

/* Network controller subclasses */
#define PCI_SUBCLASS_NETWORK_ETHERNET   0x00
#define PCI_SUBCLASS_NETWORK_TOKEN_RING 0x01
#define PCI_SUBCLASS_NETWORK_FDDI       0x02
#define PCI_SUBCLASS_NETWORK_ATM        0x03
#define PCI_SUBCLASS_NETWORK_ISDN       0x04
#define PCI_SUBCLASS_NETWORK_WORLDFIP   0x05
#define PCI_SUBCLASS_NETWORK_PICMG      0x06
#define PCI_SUBCLASS_NETWORK_INFINIBAND 0x07
#define PCI_SUBCLASS_NETWORK_FABRIC     0x08

/* Display controller subclasses */
#define PCI_SUBCLASS_DISPLAY_VGA_COMPATIBLE 0x00
#define PCI_SUBCLASS_DISPLAY_XGA            0x01
#define PCI_SUBCLASS_DISPLAY_3D             0x02

/* Multimedia controller subclasses */
#define PCI_SUBCLASS_MULTIMEDIA_VIDEO              0x00
#define PCI_SUBCLASS_MULTIMEDIA_AUDIO              0x01
#define PCI_SUBCLASS_MULTIMEDIA_COMPUTER_TELEPHONY 0x02
#define PCI_SUBCLASS_MULTIMEDIA_AUDIO_DEVICE       0x03

/* Memory controller subclasses */
#define PCI_SUBCLASS_MEMORY_RAM   0x00
#define PCI_SUBCLASS_MEMORY_FLASH 0x01
#define PCI_SUBCLASS_MEMORY_CXL   0x02

/* Bridge subclasses */
#define PCI_SUBCLASS_BRIDGE_HOST            0x00
#define PCI_SUBCLASS_BRIDGE_ISA             0x01
#define PCI_SUBCLASS_BRIDGE_EISA            0x02
#define PCI_SUBCLASS_BRIDGE_MCA             0x03
#define PCI_SUBCLASS_BRIDGE_PCI             0x04
#define PCI_SUBCLASS_BRIDGE_PCMCIA          0x05
#define PCI_SUBCLASS_BRIDGE_NUBUS           0x06
#define PCI_SUBCLASS_BRIDGE_CARDBUS         0x07
#define PCI_SUBCLASS_BRIDGE_RACEWAY         0x08
#define PCI_SUBCLASS_BRIDGE_PCI_TO_PCI_ST   0x09
#define PCI_SUBCLASS_BRIDGE_INFINIBAND_HOST 0x0a

/* Serial device controller subclasses */
#define PCI_SUBCLASS_SERIAL_FIREWIRE          0x00
#define PCI_SUBCLASS_SERIAL_ACCESS            0x01
#define PCI_SUBCLASS_SERIAL_SSA               0x02
#define PCI_SUBCLASS_SERIAL_USB               0x03
#define PCI_SUBCLASS_SERIAL_FIBRE_CHANNEL     0x04
#define PCI_SUBCLASS_SERIAL_SMBUS             0x05
#define PCI_SUBCLASS_SERIAL_SERIAL_INFINIBAND 0x06
#define PCI_SUBCLASS_SERIAL_IPMI              0x07
#define PCI_SUBCLASS_SERIAL_SERCOS            0x08
#define PCI_SUBCLASS_SERIAL_CANBUS            0x09
#define PCI_SUBCLASS_SERIAL_MIPI_I3C          0x0a

/* USB interfaces */
#define PCI_INTERFACE_USB_UHCI   0x00
#define PCI_INTERFACE_USB_OHCI   0x10
#define PCI_INTERFACE_USB_EHCI   0x20
#define PCI_INTERFACE_USB_XHCI   0x30
#define PCI_INTERFACE_USB_USB4   0x40
#define PCI_INTERFACE_USB_DEVICE 0xfe

#define PCI_HEADER_TYPE_DEVICE                0x00
#define PCI_HEADER_TYPE_PCI_TO_PCI_BRIDGE     0x01
#define PCI_HEADER_TYPE_PCI_TO_CARDBUS_BRIDGE 0x02
#define PCI_HEADER_TYPE_MULTIFUNCTION         BIT(7)

typedef struct {
    Device device;

    uint8 bus;
    uint8 slot;
    uint8 function;

    uint16 vendorId;
    uint16 deviceId;
    uint8 class;
    uint8 subclass;
    uint8 interface;
} PciDevice;

const char8 *pciGetClassString(uint8 class, uint8 subclass);

void pciInitialize();

uint32 pciGetBar(PciDevice *dev, uint8 index);
