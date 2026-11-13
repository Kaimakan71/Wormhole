/*
 * Device management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <list.h>
#include <types.h>

typedef struct {
    ListItem listItem;
    const char8 *name;
    List devices;
} DeviceClass;

typedef struct {
    ListItem listItem;
    const char8 *name;
    DeviceClass *class;
} Device;

extern DeviceClass deviceClassTimer;
extern DeviceClass deviceClassKeyboard;

void deviceInitialize();
void deviceEnumerateClasses(void (*callback)(DeviceClass *));

void deviceClassRegister(DeviceClass *class);

void deviceRegister(Device *device);
