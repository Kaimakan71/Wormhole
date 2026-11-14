/*
 * Thread management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <cpu/state.h>
#include <process.h>

typedef struct {
    ListItem listItem;

    uintn id;
    const char8 *name;
    Process *parent;

    void *stack;
    uintn stackSize;

    CpuThreadState cpuState;
} Thread;

Thread *threadCreate(Process *parent, const char8 *name);

void threadArchInitialize(Thread *thread);
