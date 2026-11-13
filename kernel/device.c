/*
 * Device management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <device.h>
#include <log.h>
#include <mm.h>

#define TRACE_SOURCE "device"

DeviceClass deviceClassTimer = {
    .name = "timer",
};

DeviceClass deviceClassKeyboard = {
    .name = "keyboard",
};

static List deviceClasses;

void deviceInitialize() {
    traceDebug("Initializing device manager...\n");
    listReset(&deviceClasses);
    deviceClassRegister(&deviceClassTimer);
    deviceClassRegister(&deviceClassKeyboard);
}

void deviceEnumerateClasses(void (*callback)(DeviceClass *)) {
    ListItem *listItem;

    listEnumerateItems(&deviceClasses, listItem) {
        callback((DeviceClass *) listItem);
    }
}

void deviceClassRegister(DeviceClass *class) {
    listReset(&class->devices);
    listAppend(&deviceClasses, &class->listItem);
}

void deviceRegister(Device *device) {
    listAppend(&device->class->devices, &device->listItem);
}
