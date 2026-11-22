/*
 * Device management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <cpu/state.h>
#include <list.h>

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

typedef struct TimerDevice TimerDevice;

typedef struct {
    void (*setFrequency)(TimerDevice *device, uint64 frequency);
} TimerDeviceOps;

typedef struct TimerDevice {
    Device device;
    const TimerDeviceOps *ops;
    CpuThreadState *(*onTick)(CpuThreadState *cpuState);
} TimerDevice;

typedef struct {
    Device device;
    void (*onKeyPress)(char8 ch);
} KeyboardDevice;

extern DeviceClass deviceClassTimer;
extern DeviceClass deviceClassKeyboard;

void deviceInitialize();
void deviceEnumerateClasses(void (*callback)(DeviceClass *));

void deviceClassRegister(DeviceClass *class);

void deviceRegister(Device *device);
